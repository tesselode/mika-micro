#include "Voice.h"

double Voice::Next(double dt)
{
	volEnv.Update(dt, 10.0, 10.0, 0, 10.0);
	auto out = 0.0;
	out = osc1a.Next(dt, baseFrequency, Waveforms::Sine);
	out *= volEnv.Get();
	return out;
}
