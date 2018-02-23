#include "Voice.h"

void Voice::CalculateFrequencies()
{
	auto baseFrequency = pitchToFrequency(note);
	osc1aFrequency = baseFrequency * osc1SplitFactorA;
	osc1bFrequency = baseFrequency * osc1SplitFactorB;
	osc2aFrequency = baseFrequency * osc2SplitFactorA;
	osc2bFrequency = baseFrequency * osc2SplitFactorB;
}

double Voice::GetOsc1()
{
	if (osc1SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc1a.Next(osc1aFrequency, OscillatorWaveformSaw);
		out += osc1b.Next(osc1bFrequency, OscillatorWaveformSaw);
		return out;
	}
	return osc1a.Next(osc1aFrequency, OscillatorWaveformSaw);
}

double Voice::GetOsc2()
{
	if (osc2SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc2a.Next(osc2aFrequency, OscillatorWaveformSaw);
		out += osc2b.Next(osc2bFrequency, OscillatorWaveformSaw);
		return out;
	}
	return osc2a.Next(osc1aFrequency, OscillatorWaveformSaw);
}

double Voice::GetOscillators()
{
	auto out = 0.0;
	out += GetOsc1();
	out += GetOsc2();
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
