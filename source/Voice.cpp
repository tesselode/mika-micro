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

double Voice::GetOsc1Frequency(std::vector<double> &parameters, double fm)
{
	int p = note + parameters[osc1Coarse] + parameters[osc1Fine];
	if (parameters[fmCoarse] < 0.0)
		p -= fm * (parameters[fmCoarse] + parameters[fmFine]);
	return PitchToFrequency(p);
}

double Voice::GetOsc2Frequency(std::vector<double> &parameters, double fm)
{
	int p = note + parameters[osc2Coarse] + parameters[osc2Fine];
	if (parameters[fmCoarse] > 0.0)
		p += fm * (parameters[fmCoarse] + parameters[fmFine]);
	return PitchToFrequency(p);
}

double Voice::GetOscFm(double dt, std::vector<double>& parameters)
{
	return oscFm.Next(dt, GetOsc1Frequency(parameters, 0.0), OscillatorWaveformSine);
}

double Voice::GetOsc1(double dt, std::vector<double> &parameters, double fm)
{
	if (parameters[osc1Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc1Frequency(parameters, fm);
		out += osc1a.Next(dt, frequency / (1 + parameters[osc1Split]), OscillatorWaveformSaw);
		out += osc1b.Next(dt, frequency * (1 + parameters[osc1Split]), OscillatorWaveformSaw);
		return out;
	}
	return osc1a.Next(dt, GetOsc1Frequency(parameters, fm), OscillatorWaveformSaw);
}

double Voice::GetOsc2(double dt, std::vector<double> &parameters, double fm)
{
	if (parameters[osc2Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc2Frequency(parameters, fm);
		out += osc2a.Next(dt, frequency / (1 + parameters[osc2Split]), OscillatorWaveformSaw);
		out += osc2b.Next(dt, frequency * (1 + parameters[osc2Split]), OscillatorWaveformSaw);
		return out;
	}
	return osc2a.Next(dt, GetOsc2Frequency(parameters, fm), OscillatorWaveformSaw);
}

double Voice::GetOscillators(double dt, std::vector<double> &parameters)
{
	auto fm = GetOscFm(dt, parameters);
	double out = 0.0;
	if (parameters[oscMix] < 1.0)
		out += GetOsc1(dt, parameters, fm) * (1 - parameters[oscMix]);
	if (parameters[oscMix] > 0.0)
		out += GetOsc2(dt, parameters, fm) * parameters[oscMix];
	return out;
}

double Voice::Next(double dt, std::vector<double> &parameters)
{
	volumeEnvelope.Update(dt, parameters[volEnvA], parameters[volEnvD], parameters[volEnvS], parameters[volEnvR]);
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators(dt, parameters);
	out *= GetVolume();
	return out;
}