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

void Voice::Start()
{
	volumeEnvelope.Start();
}

void Voice::Release()
{
	volumeEnvelope.Release();
}

double Voice::GetVolume()
{
	return 0.0;
}

bool Voice::IsReleased()
{
	return volumeEnvelope.IsReleased();
}

double Voice::Next(double dt)
{
	double out = 0.0;
	out += osc1.Next(dt, PitchToFrequency(note), OscillatorWaveformSaw);
	volumeEnvelope.Update(dt, 1.0, 1.0, 0.5, 10.0);
	out *= volumeEnvelope.Get();
	return out;
}