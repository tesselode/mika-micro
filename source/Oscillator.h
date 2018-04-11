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
		previousWaveformMix = currentWaveformMix;
		currentWaveform = waveform;
		currentWaveformMix = 0.0;
	}
	void Reset(double p = 0.0) 
	{
		phase = p;
		previousWaveformMix = 0.0;
		currentWaveformMix = 1.0;
	}
	double Next(double dt, double frequency);

private:
	double Blep(double phase);
	double GeneratePulse(double width);
	double Get(EWaveforms waveform);

	EWaveforms previousWaveform = kNoWaveform;
	double previousWaveformMix = 0.0;
	EWaveforms currentWaveform = kSine;
	double currentWaveformMix = 1.0;

	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

