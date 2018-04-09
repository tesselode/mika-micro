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
	kNumWaveforms
};

class Oscillator
{
public:
	void Reset(double p = 0.0) { phase = p; }
	double Next(double dt, double frequency, EWaveforms waveform);

private:
	double Blep(double phase);
	double GeneratePulse(double width);

	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

