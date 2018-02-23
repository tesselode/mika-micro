#include "Voice.h"

double Voice::Next()
{
	auto out = 0.0;
	out += osc1a.Next(baseFrequency, OscillatorWaveformSine);
	return out;
}
