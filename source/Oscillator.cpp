#include "Oscillator.h"

void Oscillator::Update()
{
	dt = frequency / sampleRate;
	t += dt;
	t = fmod(t, 1.0);
}

// polyBLEP oscillator
// http://www.kvraudio.com/forum/viewtopic.php?t=375517

double Oscillator::Blep(double t)
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

double Oscillator::Get(Waveform waveform)
{
	if (waveform == Sine)
	{
		return sin(t * twoPi);
	}
	if (waveform == Saw)
	{
		return 1 - 2 * t + Blep(t);
	}
}