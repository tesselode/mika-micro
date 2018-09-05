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
	NumWaveforms
};

class Oscillator
{
public:
	double Next(double dt, double frequency, double sineMix, double triangleMix,
		double sawMix, double squareMix, double pulseMix, double noiseMix);

private:
	double Blep(double phase);
	double GeneratePulse(double width);
	void UpdatePhase(double dt, double frequency);

	double phaseIncrement = 0.0;
	double phase = 0.0;
	double triLast = 0.0;
	double triCurrent = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

