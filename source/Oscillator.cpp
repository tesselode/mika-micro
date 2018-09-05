#include "Oscillator.h"

double Oscillator::Next(double dt, double frequency, double sineMix, double triangleMix, double sawMix,
	double squareMix, double pulseMix, double noiseMix)
{
	phase += frequency * dt;
	while (phase > 1.0) phase -= 1.0;

	auto out = 0.0;
	if (sineMix > 0.0) out += sin(phase * twoPi) * sineMix;
	return out;
}
