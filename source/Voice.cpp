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

double Voice::GetOsc1Frequency(std::vector<double> &parameters)
{
	return PitchToFrequency(note + parameters[osc1Coarse] + parameters[osc1Fine]);
}

double Voice::GetOsc2Frequency(std::vector<double> &parameters)
{
	return PitchToFrequency(note + parameters[osc2Coarse] + parameters[osc2Fine]);
}

double Voice::GetOsc1(double dt, std::vector<double> &parameters)
{
	return osc1.Next(dt, GetOsc1Frequency(parameters), OscillatorWaveformSaw);
}

double Voice::GetOsc2(double dt, std::vector<double> &parameters)
{
	return osc2.Next(dt, GetOsc2Frequency(parameters), OscillatorWaveformSaw);
}

double Voice::GetOscillators(double dt, std::vector<double> &parameters)
{
	double out = 0.0;
	if (parameters[oscMix] < 1.0)
		out += GetOsc1(dt, parameters) * (1 - parameters[oscMix]);
	if (parameters[oscMix] > 0.0)
		out += GetOsc2(dt, parameters) * parameters[oscMix];
	return out;
}

double Voice::Next(double dt, std::vector<double> &parameters)
{
	volumeEnvelope.Update(dt, parameters[volEnvA], parameters[volEnvD], parameters[volEnvS], parameters[volEnvR]);
	if (GetVolume() == 0.0) return 0.0;

	double out = GetOscillators(dt, parameters);
	out *= GetVolume();
	return out;
}