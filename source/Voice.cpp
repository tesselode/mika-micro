#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	targetFrequency = pitchToFrequency(note);
}

void Voice::Reset()
{
	oscFm.Reset();
	osc1a.Reset();
	osc1b.Reset(p[(int)Parameters::Osc1Split] < 0.0 ? .33 : 0.0);
	osc2a.Reset();
	osc2b.Reset(p[(int)Parameters::Osc2Split] < 0.0 ? .33 : 0.0);
	volEnv.Reset();
	modEnv.Reset();
	lfoEnv.Reset();

	// set smoothed variables directly to their target values
	osc1bMix = p[(int)Parameters::Osc1Split] != 0.0 ? 1.0 : 0.0;
	osc2bMix = p[(int)Parameters::Osc2Split] != 0.0 ? 1.0 : 0.0;
}

void Voice::Start()
{
	if (GetVolume() == 0.0) Reset();
	volEnv.Start();
	modEnv.Start();
	lfoEnv.Start();
}

void Voice::Release()
{
	volEnv.Release();
	modEnv.Release();
	lfoEnv.Release();
}

void Voice::UpdateEnvelopes(double dt)
{
	volEnv.Update(dt, p[(int)Parameters::VolEnvA], p[(int)Parameters::VolEnvD], p[(int)Parameters::VolEnvS], p[(int)Parameters::VolEnvR]);
	modEnv.Update(dt, p[(int)Parameters::ModEnvA], p[(int)Parameters::ModEnvD], p[(int)Parameters::ModEnvS], p[(int)Parameters::ModEnvR]);
	lfoEnv.Update(dt, p[(int)Parameters::LfoDelay], 0.5, 1.0, 0.5);
}

double Voice::GetFilterCutoff(double volEnvValue, double modEnvValue, double lfoValue, double driftValue)
{
	auto cutoff = p[(int)Parameters::FilterCutoff];
	cutoff *= 1.0 + driftValue;
	cutoff += p[(int)Parameters::VolEnvCutoff] * volEnvValue;
	cutoff += p[(int)Parameters::ModEnvCutoff] * modEnvValue;
	cutoff += lfoValue * p[(int)Parameters::LfoCutoff];
	cutoff += p[(int)Parameters::FilterKeyTrack] * baseFrequency * pitchBendFactor;
	return cutoff;
}

double Voice::Next(double dt, double lfoValue, double driftValue)
{
	// update envelopes
	UpdateEnvelopes(dt);
	auto volEnvValue = volEnv.Get(p[(int)Parameters::VolEnvV], velocity);
	auto modEnvValue = modEnv.Get(p[(int)Parameters::ModEnvV], velocity);

	// skip processing if voice is silent
	if (volEnvValue == 0.0 && filter.IsSilent()) return 0.0;

	// apply lfo delay
	lfoValue *= lfoEnv.Get(0.0, velocity);

	// glide to target frequency (for mono and legato modes)
	baseFrequency += (targetFrequency - baseFrequency) * p[(int)Parameters::GlideLength] * dt;

	// smooth oscillator split
	auto osc1bEnabled = p[(int)Parameters::Osc1Split] != 0.0 && osc1b.GetWaveform() != Waveforms::Noise;
	auto osc2bEnabled = p[(int)Parameters::Osc2Split] != 0.0 && osc2b.GetWaveform() != Waveforms::Noise;
	osc1bMix += ((osc1bEnabled ? 1.0 : 0.0) - osc1bMix) * 100.0 * dt;
	osc2bMix += ((osc2bEnabled ? 1.0 : 0.0) - osc2bMix) * 100.0 * dt;

	// calculate oscillator base frequencies
	auto osc1Frequency = baseFrequency * osc1PitchFactor * pitchBendFactor * (1.0 + driftValue);
	if (p[(int)Parameters::LfoAmount] < 0.0) osc1Frequency *= 1.0 + abs(p[(int)Parameters::LfoAmount]) * lfoValue;
	auto osc2Frequency = baseFrequency * osc2PitchFactor * pitchBendFactor * (1.0 + driftValue);
	osc2Frequency *= 1.0 + abs(p[(int)Parameters::LfoAmount]) * lfoValue;

	// fm
	auto fmMode = (FmModes)(int)p[(int)Parameters::FmMode];
	switch (fmMode)
	{
	case FmModes::Osc1:
	case FmModes::Osc2:
	{
		auto fmAmount = p[(int)Parameters::FmCoarse] + p[(int)Parameters::FmFine];
		fmAmount += p[(int)Parameters::VolEnvFm] * volEnvValue;
		fmAmount += p[(int)Parameters::ModEnvFm] * modEnvValue;
		fmAmount += p[(int)Parameters::LfoFm] * lfoValue;
		auto fmValue = pitchFactor(oscFm.Next(dt, osc1Frequency) * fmAmount);
		switch (fmMode)
		{
		case FmModes::Osc1:
			osc1Frequency *= fmValue;
			break;
		case FmModes::Osc2:
			osc2Frequency *= fmValue;
			break;
		}
		break;
	}
	}

	auto out = 0.0;

	// oscillator 1
	auto osc1Out = 0.0;
	if (p[(int)Parameters::OscMix] < .99)
	{
		osc1Out += osc1a.Next(dt, osc1Frequency * osc1SplitFactorA);
		if (osc1bMix > .01)
			osc1Out += osc1bMix * osc1b.Next(dt, osc1Frequency * osc1SplitFactorB);
		out += osc1Out * sqrt(1.0 - p[(int)Parameters::OscMix]);
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[(int)Parameters::OscMix] > .01)
	{
		osc2Out += osc2a.Next(dt, osc2Frequency * osc2SplitFactorA);
		if (osc2bMix > .01)
			osc2Out += osc2bMix * osc2b.Next(dt, osc2Frequency * osc2SplitFactorB);
		out += osc2Out * sqrt(p[(int)Parameters::OscMix]);
	}

	// apply volume envelope
	out *= GetVolume();

	// filter
	auto cutoff = GetFilterCutoff(volEnvValue, modEnvValue, lfoValue, driftValue);
	out = filter.Process(dt, out, cutoff, p[(int)Parameters::FilterResonance]);

	return out;
}
