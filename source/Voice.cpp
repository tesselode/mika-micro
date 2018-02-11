#include "Voice.h"

double Voice::PitchFactor(double p) { return pow(1.0595, p); }
double Voice::PitchToFrequency(double p) { return 440.0 * PitchFactor(p - 69); }

int Voice::GetNote() { return note; }
void Voice::SetNote(int n) { note = n; }
double Voice::GetVolume() { return volumeEnvelope.Get(); }
bool Voice::IsReleased() { return volumeEnvelope.IsReleased(); }

void Voice::Start()
{
	volumeEnvelope.Start();
}

void Voice::Release()
{
	volumeEnvelope.Release();
}

double Voice::Next(double dt, std::vector<double> &parameters)
{
	volumeEnvelope.Update(dt, parameters[volEnvA], parameters[volEnvD], parameters[volEnvS], parameters[volEnvR]);
	if (GetVolume() == 0.0) return 0.0;

	double out = 0.0;
	out += osc1.Next(dt, PitchToFrequency(note), OscillatorWaveformSaw);
	out *= GetVolume();
	return out;
}