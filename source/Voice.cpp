#include "Voice.h"

double Voice::PitchFactor(double p)
{
	return pow(1.0595, p);
}

double Voice::PitchToFrequency(double p)
{
	return 440.0 * PitchFactor(p - 69);
}

int Voice::GetNote()
{
	return note;
}

void Voice::SetNote(int n)
{
	note = n;
}

double Voice::Next(double dt)
{
	double out = 0.0;
	out += osc1.Next(dt, PitchToFrequency(note), OscillatorWaveformSaw);
	return out;
}