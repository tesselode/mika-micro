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
	double phase = 0.0;
};

