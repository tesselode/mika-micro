#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	targetFrequency = pitchToFrequency(note);
}

void Voice::Reset()
{
	oscFm.ResetPhase();
	osc1a.ResetPhase();
	osc1b.ResetPhase(parameters[(int)InternalParameters::Osc1SplitFactorA]->Get() < 1.0 ? .33 : 0.0);
	osc2a.ResetPhase();
	osc2b.ResetPhase(parameters[(int)InternalParameters::Osc1SplitFactorA]->Get() < 1.0 ? .33 : 0.0);
	volEnv.Reset();
}

void Voice::Start()
{
	if (GetVolume() == 0.0) Reset();
	volEnv.Start();
}

void Voice::UpdateEnvelopes(double dt)
{
	volEnv.Update(
		dt,
		parameters[(int)InternalParameters::VolEnvA]->Get(),
		parameters[(int)InternalParameters::VolEnvD]->Get(),
		parameters[(int)InternalParameters::VolEnvS]->Get(),
		parameters[(int)InternalParameters::VolEnvR]->Get()
	);
}

void Voice::UpdateGlide(double dt)
{
	auto glideLength = parameters[(int)InternalParameters::GlideLength]->Get();
	baseFrequency += (targetFrequency - baseFrequency) * glideLength * dt;
}

double Voice::GetFmMultiplier(double dt)
{
	auto fmCoarse = parameters[(int)InternalParameters::FmCoarse]->Get();
	auto fmFine = parameters[(int)InternalParameters::FmFine]->Get();
	auto fmAmount = fmCoarse + fmFine;
	return pitchFactor(oscFm.Next(dt, GetOscillator1Frequency(dt, true), 1.0, 0.0, 0.0, 0.0, 0.0, 0.0) * fmAmount);
}

double Voice::GetOscillator1Frequency(double dt, bool skipFm)
{
	auto coarse = parameters[(int)InternalParameters::Osc1Coarse]->Get();
	auto fine = parameters[(int)InternalParameters::Osc1Fine]->Get();
	auto frequency = baseFrequency * coarse * fine;
	switch (skipFm)
	{
	case false:
	{
		auto fmMode = (FmModes)(int)parameters[(int)InternalParameters::FmMode]->Get();
		switch (fmMode)
		{
		case FmModes::Osc1:
			frequency *= GetFmMultiplier(dt);
			break;
		}
		break;
	}
	}
	return frequency;
}

double Voice::GetOscillator2Frequency(double dt)
{
	auto coarse = parameters[(int)InternalParameters::Osc2Coarse]->Get();
	auto fine = parameters[(int)InternalParameters::Osc2Fine]->Get();
	auto frequency = baseFrequency * coarse * fine;
	auto fmMode = (FmModes)(int)parameters[(int)InternalParameters::FmMode]->Get();
	switch (fmMode)
	{
	case FmModes::Osc2:
		frequency *= GetFmMultiplier(dt);
		break;
	}
	return frequency;
}

double Voice::GetOscillator1(double dt)
{
	auto frequency = GetOscillator1Frequency(dt);
	auto splitA = parameters[(int)InternalParameters::Osc1SplitFactorA]->Get();
	auto splitB = parameters[(int)InternalParameters::Osc1SplitFactorB]->Get();

	auto out = 0.0;

	out += osc1a.Next(
		dt,
		frequency * splitA,
		parameters[(int)InternalParameters::Osc1SineMix]->Get(),
		parameters[(int)InternalParameters::Osc1TriangleMix]->Get(),
		parameters[(int)InternalParameters::Osc1SawMix]->Get(),
		parameters[(int)InternalParameters::Osc1SquareMix]->Get(),
		parameters[(int)InternalParameters::Osc1PulseMix]->Get(),
		parameters[(int)InternalParameters::Osc1NoiseMix]->Get()
	);

	auto splitMix = parameters[(int)InternalParameters::Osc1SplitMix]->Get();
	if (splitMix > 0.0)
		out += splitMix * osc1b.Next(
			dt,
			frequency * splitB,
			parameters[(int)InternalParameters::Osc1SineMix]->Get(),
			parameters[(int)InternalParameters::Osc1TriangleMix]->Get(),
			parameters[(int)InternalParameters::Osc1SawMix]->Get(),
			parameters[(int)InternalParameters::Osc1SquareMix]->Get(),
			parameters[(int)InternalParameters::Osc1PulseMix]->Get(),
			parameters[(int)InternalParameters::Osc1NoiseMix]->Get()
		);

	return out;
}

double Voice::GetOscillator2(double dt)
{
	auto frequency = GetOscillator2Frequency(dt);
	auto splitA = parameters[(int)InternalParameters::Osc2SplitFactorA]->Get();
	auto splitB = parameters[(int)InternalParameters::Osc2SplitFactorB]->Get();

	auto out = 0.0;

	out += osc2a.Next(
		dt,
		frequency * splitA,
		parameters[(int)InternalParameters::Osc2SineMix]->Get(),
		parameters[(int)InternalParameters::Osc2TriangleMix]->Get(),
		parameters[(int)InternalParameters::Osc2SawMix]->Get(),
		parameters[(int)InternalParameters::Osc2SquareMix]->Get(),
		parameters[(int)InternalParameters::Osc2PulseMix]->Get(),
		parameters[(int)InternalParameters::Osc2NoiseMix]->Get()
	);

	auto splitMix = parameters[(int)InternalParameters::Osc2SplitMix]->Get();
	if (splitMix > 0.0)
		out += splitMix * osc2b.Next(
			dt,
			frequency * splitB,
			parameters[(int)InternalParameters::Osc2SineMix]->Get(),
			parameters[(int)InternalParameters::Osc2TriangleMix]->Get(),
			parameters[(int)InternalParameters::Osc2SawMix]->Get(),
			parameters[(int)InternalParameters::Osc2SquareMix]->Get(),
			parameters[(int)InternalParameters::Osc2PulseMix]->Get(),
			parameters[(int)InternalParameters::Osc2NoiseMix]->Get()
		);

	return out;
}

double Voice::GetOscillators(double dt)
{
	auto oscMix = parameters[(int)InternalParameters::OscMix]->Get();
	auto out = 0.0;
	if (oscMix < 1.0)
		out += GetOscillator1(dt) * sqrt(1.0 - oscMix);
	if (oscMix > 0.0)
		out += GetOscillator2(dt) * sqrt(oscMix);
	return out;
}

double Voice::ApplyFilter(double dt, double input)
{
	auto dryMix = parameters[(int)InternalParameters::FilterDryMix]->Get();
	auto twoPoleMix = parameters[(int)InternalParameters::Filter2pMix]->Get();
	auto svfMix = parameters[(int)InternalParameters::FilterSvfMix]->Get();
	auto fourPoleMix = parameters[(int)InternalParameters::Filter4pMix]->Get();

	if (dryMix == 1.0) return input;

	auto cutoff = parameters[(int)InternalParameters::FilterCutoff]->Get();
	auto resonance = parameters[(int)InternalParameters::FilterResonance]->Get();
	auto out = 0.0;
	out += input * dryMix;
	if (twoPoleMix > 0.0) out += twoPoleMix * twoPole.Process(dt, input, cutoff, resonance);
	if (svfMix > 0.0) out += svfMix * svf.Process(dt, input, cutoff, resonance);
	if (fourPoleMix > 0.0) out += fourPoleMix * fourPole.Process(dt, input, cutoff, resonance);
	return out;
}

double Voice::Next(double dt)
{
	UpdateEnvelopes(dt);
	UpdateGlide(dt);
	auto out = GetOscillators(dt);
	out *= volEnv.Get();
	out = ApplyFilter(dt, out);
	return out;
}
