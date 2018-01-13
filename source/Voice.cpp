#include "Voice.h"

double Voice::Sine(double t)
{
	return sin(t * twoPi);
}

void Voice::SetNote(int n)
{
	note = n;
	frequency = PitchToFrequency(n);
}

double Voice::Next()
{
	modEnvelope.Update();

	phase1a = fmod(phase1a + frequency / sampleRate, 1.0);
	phase1b = fmod(phase1b + frequency * 1.01 / sampleRate, 1.0);
	phase2a = fmod(phase2a + frequency / 1.01 / sampleRate, 1.0);
	phase2b = fmod(phase2b + frequency * 1.02 / sampleRate, 1.0);

	double out = 0.0;
	out += Sine(phase1a);
	out += Sine(phase1b);
	out += Sine(phase2a);
	out += Sine(phase2b);

	out *= modEnvelope.Get();

	return out;
}