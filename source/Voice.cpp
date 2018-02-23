#include "Voice.h"

double Voice::GetOscillators()
{
	auto out = 0.0;
	out += osc1a.Next(baseFrequency, OscillatorWaveformSine);
	return out;
}

double Voice::Next()
{
	UpdateEnvelopes();
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators();
	out *= GetVolume();
	return out;
}
