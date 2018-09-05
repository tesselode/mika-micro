#include "Voice.h"

void Voice::Reset()
{
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

double Voice::GetOscillator1(double dt)
{
	auto coarse = parameters[(int)InternalParameters::Osc1Coarse]->Get();
	auto fine = parameters[(int)InternalParameters::Osc1Fine]->Get();
	auto splitA = parameters[(int)InternalParameters::Osc1SplitFactorA]->Get();
	auto splitB = parameters[(int)InternalParameters::Osc1SplitFactorB]->Get();

	auto out = 0.0;

	out += osc1a.Next(
		dt,
		baseFrequency * coarse * fine * splitA,
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
			baseFrequency * coarse * fine * splitB,
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
	auto coarse = parameters[(int)InternalParameters::Osc2Coarse]->Get();
	auto fine = parameters[(int)InternalParameters::Osc2Fine]->Get();
	auto splitA = parameters[(int)InternalParameters::Osc2SplitFactorA]->Get();
	auto splitB = parameters[(int)InternalParameters::Osc2SplitFactorB]->Get();

	auto out = 0.0;

	out += osc2a.Next(
		dt,
		baseFrequency * coarse * fine * splitA,
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
			baseFrequency * coarse * fine * splitB,
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

double Voice::Next(double dt)
{
	UpdateEnvelopes(dt);
	auto out = GetOscillators(dt);
	out *= volEnv.Get();
	return out;
}
