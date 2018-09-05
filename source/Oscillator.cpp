#include "Oscillator.h"

// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double Oscillator::Blep(double phase)
{
	if (phase < phaseIncrement)
	{
		phase /= phaseIncrement;
		return phase + phase - phase * phase - 1.0;
	}
	else if (phase > 1.0 - phaseIncrement)
	{
		phase = (phase - 1.0) / phaseIncrement;
		return phase * phase + phase + phase + 1.0;
	}
	return 0.0;
}

double Oscillator::GeneratePulse(double width)
{
	double v = phase < width ? 1.0 : -1.0;
	v += Blep(phase);
	v -= Blep(fmod(phase + (1.0 - width), 1.0));
	return v;
}

void Oscillator::UpdatePhase(double dt, double frequency)
{
	phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase > 1.0) phase -= 1.0;
}

double Oscillator::Next(double dt, double frequency, double sineMix, double triangleMix, double sawMix,
	double squareMix, double pulseMix, double noiseMix)
{
	UpdatePhase(dt, frequency);

	auto out = 0.0;
	if (sineMix > 0.0) out += sin(phase * twoPi) * sineMix;
	if (triangleMix > 0.0)
	{
		triLast = triCurrent;
		triCurrent = phaseIncrement * GeneratePulse(.5) + (1.0 - phaseIncrement) * triLast;
		out += triCurrent * 5.0 * triangleMix;
	}
	if (sawMix > 0.0) out += (1.0 - 2.0 * phase + Blep(phase)) * sawMix;
	if (squareMix > 0.0) out += GeneratePulse(.5) * squareMix;
	if (pulseMix > 0.0) out += GeneratePulse(.75) * pulseMix;
	if (noiseMix > 0.0)
	{
		noiseValue += 19.0;
		noiseValue *= noiseValue;
		noiseValue -= (int)noiseValue;
		out += (noiseValue - .5) * noiseMix;
	}
	return out;
}
