#include "Voice.h"

void Voice::UpdateEnvelopes(double dt)
{
	volEnv.Update(
		dt,
		parameters[(int)Parameters::VolEnvA]->Get(),
		parameters[(int)Parameters::VolEnvD]->Get(),
		parameters[(int)Parameters::VolEnvS]->Get(),
		parameters[(int)Parameters::VolEnvR]->Get()
	);
}

double Voice::Next(double dt)
{
	UpdateEnvelopes(dt);
	auto out = 0.0;
	out = osc1a.Next(dt, baseFrequency, Waveforms::Sine);
	out *= volEnv.Get();
	return out;
}