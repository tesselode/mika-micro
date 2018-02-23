#include "Voice.h"

double Voice::GetOsc1()
{
	if (osc1SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc1a.Next(baseFrequency * osc1SplitFactorA, OscillatorWaveformSaw);
		out += osc1b.Next(baseFrequency * osc1SplitFactorB, OscillatorWaveformSaw);
		return out;
	}
	return osc1a.Next(baseFrequency, OscillatorWaveformSaw);
}

double Voice::GetOsc2()
{
	if (osc2SplitFactorA != 1.0)
	{
		auto out = 0.0;
		out += osc2a.Next(baseFrequency * osc2SplitFactorA, OscillatorWaveformSaw);
		out += osc2b.Next(baseFrequency * osc2SplitFactorB, OscillatorWaveformSaw);
		return out;
	}
	return osc2a.Next(baseFrequency, OscillatorWaveformSaw);
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
