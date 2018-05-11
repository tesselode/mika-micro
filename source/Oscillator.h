#pragma once

#include <cmath>
#include "Util.h"

enum EWaveforms
{
	kSine,
	kTriangle,
	kSaw,
	kSquare,
	kPulse,
	kNoise,
	kNumWaveforms,
	kNoWaveform
};

class Oscillator
{
public:
	void SetWaveform(EWaveforms waveform)
	{
		previousWaveform = currentWaveform;
		currentWaveform = waveform;
		crossfading = true;
		currentWaveformMix = 0.0;
	}
	EWaveforms GetWaveform() { return currentWaveform; }
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
	double Get(EWaveforms waveform);
	void UpdatePhase(double dt, double frequency);

	EWaveforms previousWaveform = kNoWaveform;
	EWaveforms currentWaveform = kSine;
	bool crossfading = false;
	double currentWaveformMix = 1.0;

	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

