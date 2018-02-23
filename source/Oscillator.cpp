#include "Oscillator.h"

// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double Oscillator::Blep(double t, double dt)
{
	if (t < dt)
	{
		t /= dt;
		return t + t - t * t - 1.0;
	}
	else if (t > 1.0 - dt)
	{
		t = (t - 1.0) / dt;
		return t * t + t + t + 1.0;
	}
	return 0.0;
}

double Oscillator::Next(double frequency, OscillatorWaveform waveform)
{
	double phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase >= 1) phase -= 1;
	
	switch (waveform)
	{
	case OscillatorWaveformSine:
		return sin(phase * twoPi);
	case OscillatorWaveformSaw:
		return 1 - 2 * phase + Blep(phase, phaseIncrement);
	}
}
