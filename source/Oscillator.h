#pragma once

#include <cmath>
#include "Util.h"

enum class Waveforms
{
	Sine,
	Triangle,
	Saw,
	Square,
	Pulse,
	Noise,
	NumWaveforms,
	NoWaveform
};

enum class FmModes
{
	Off,
	Osc1,
	Osc2,
	NumFmModes
};

class Oscillator
{
public:
	void SetWaveform(Waveforms waveform)
	{
		previousWaveform = currentWaveform;
		currentWaveform = waveform;
		crossfading = true;
		currentWaveformMix = 0.0;
	}
	Waveforms GetWaveform() { return currentWaveform; }
	void Reset(double p = 0.0) 
	{
		phase = p;
		crossfading = false;
		currentWaveformMix = 1.0;
	}
	double Next(double dt, double frequency);

private:
	double Blep(double phase);
	double GeneratePulse(double width);
	double Get(Waveforms waveform);
	void UpdatePhase(double dt, double frequency);

	Waveforms previousWaveform = Waveforms::NoWaveform;
	Waveforms currentWaveform = Waveforms::Sine;
	bool crossfading = false;
	double currentWaveformMix = 1.0;

	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

