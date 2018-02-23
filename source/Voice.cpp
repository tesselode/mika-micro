#include "Voice.h"

double Voice::GetOscillators()
{
	double osc1BaseFrequency = baseFrequency * osc1TuneFactor;
	double osc2BaseFrequency = baseFrequency * osc2TuneFactor;

	// fm
	double fmFactor = 1.0;
	if (fmCoarse != 0)
	{
		double fmAmount = fabs(fmCoarse) + fmFine;
		double fmValue = oscFm.Next(osc1BaseFrequency, OscillatorWaveformSine);
		fmFactor = pitchFactor(fmAmount * fmValue);
	}

	// osc 1
	if (fmCoarse < 0) osc1BaseFrequency *= fmFactor;
	auto osc1Out = 0.0;
	if (osc1SplitFactorA != 1.0)
	{
		osc1Out += osc1a.Next(osc1BaseFrequency * osc1SplitFactorA, OscillatorWaveformSaw);
		osc1Out += osc1b.Next(osc1BaseFrequency * osc1SplitFactorB, OscillatorWaveformSaw);
	}
	else
		osc1Out = osc1a.Next(osc1BaseFrequency, OscillatorWaveformSaw);

	// osc 2
	if (fmCoarse > 0) osc2BaseFrequency *= fmFactor;
	auto osc2Out = 0.0;
	if (osc2SplitFactorA != 1.0)
	{
		osc2Out += osc2a.Next(osc2BaseFrequency * osc2SplitFactorA, OscillatorWaveformSaw);
		osc2Out += osc2b.Next(osc2BaseFrequency * osc2SplitFactorB, OscillatorWaveformSaw);
	}
	else
		osc2Out = osc2a.Next(osc2BaseFrequency, OscillatorWaveformSaw);

	// mix
	return .5 * osc1Out + .5 * osc2Out;
}

double Voice::Next()
{
	UpdateEnvelopes();
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators();
	out *= GetVolume();
	return out;
}
