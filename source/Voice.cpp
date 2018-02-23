#include "Voice.h"

void Voice::CalculateFrequencies()
{
	auto baseFrequency = pitchToFrequency(note);
	oscFmFrequency = baseFrequency;
	osc1aFrequency = baseFrequency * osc1SplitFactorA;
	osc1bFrequency = baseFrequency * osc1SplitFactorB;
	osc2aFrequency = baseFrequency * osc2SplitFactorA;
	osc2bFrequency = baseFrequency * osc2SplitFactorB;
}

double Voice::GetOsc1(double fmFactor)
{
	auto fa = osc1aFrequency;
	auto fb = osc1bFrequency;
	if (fmFactor != 1.0)
	{
		fa *= fmFactor;
		fb *= fmFactor;
	}
	if (osc1SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc1a.Next(fa, OscillatorWaveformSaw);
		out += osc1b.Next(fb, OscillatorWaveformSaw);
		return out;
	}
	return osc1a.Next(fa, OscillatorWaveformSaw);
}

double Voice::GetOsc2(double fmFactor)
{
	auto fa = osc2aFrequency;
	auto fb = osc2bFrequency;
	if (fmFactor != 1.0)
	{
		fa *= fmFactor;
		fb *= fmFactor;
	}
	if (osc2SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc2a.Next(fa, OscillatorWaveformSaw);
		out += osc2b.Next(fb, OscillatorWaveformSaw);
		return out;
	}
	return osc2a.Next(fa, OscillatorWaveformSaw);
}

double Voice::GetOscillators()
{
	double fmFactor = 1.0;
	if (fmCoarse != 0)
	{
		double fmAmount = fabs(fmCoarse) + fmFine;
		double fmValue = oscFm.Next(oscFmFrequency, OscillatorWaveformSine);
		fmFactor = pitchFactor(fmAmount * fmValue);
	}
	auto out = 0.0;
	out += GetOsc1(fmCoarse < 0 ? fmFactor : 1.0);
	out += GetOsc2(fmCoarse > 0 ? fmFactor : 1.0);
	return out;
}

double Voice::Next()
{
	UpdateEnvelopes();
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators();
	out *= GetVolume();
	return out;
}
