#include "Voice.h"

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

double Voice::GetOscillators(double dt)
{
	auto osc1CoarseFactor = parameters[(int)InternalParameters::Osc1Coarse]->Get();
	auto osc1FineFactor = parameters[(int)InternalParameters::Osc1Fine]->Get();
	auto osc1SplitFactorA = parameters[(int)InternalParameters::Osc1SplitFactorA]->Get();
	auto osc1SplitFactorB = parameters[(int)InternalParameters::Osc1SplitFactorB]->Get();
	
	auto out = 0.0;

	out += osc1a.Next(
		dt,
		baseFrequency * osc1CoarseFactor * osc1FineFactor * osc1SplitFactorA,
		parameters[(int)InternalParameters::Osc1SineMix]->Get(),
		parameters[(int)InternalParameters::Osc1TriangleMix]->Get(),
		parameters[(int)InternalParameters::Osc1SawMix]->Get(),
		parameters[(int)InternalParameters::Osc1SquareMix]->Get(),
		parameters[(int)InternalParameters::Osc1PulseMix]->Get(),
		parameters[(int)InternalParameters::Osc1NoiseMix]->Get()
	);

	auto osc1SplitMix = parameters[(int)InternalParameters::Osc1SplitMix]->Get();
	if (osc1SplitMix > 0.0)
		out += osc1SplitMix * osc1b.Next(
			dt,
			baseFrequency * osc1CoarseFactor * osc1FineFactor * osc1SplitFactorB,
			parameters[(int)InternalParameters::Osc1SineMix]->Get(),
			parameters[(int)InternalParameters::Osc1TriangleMix]->Get(),
			parameters[(int)InternalParameters::Osc1SawMix]->Get(),
			parameters[(int)InternalParameters::Osc1SquareMix]->Get(),
			parameters[(int)InternalParameters::Osc1PulseMix]->Get(),
			parameters[(int)InternalParameters::Osc1NoiseMix]->Get()
		);

	return out;
}

double Voice::Next(double dt)
{
	UpdateEnvelopes(dt);
	auto out = GetOscillators(dt);
	out *= volEnv.Get();
	return out;
}