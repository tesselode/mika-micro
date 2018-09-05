#pragma once

#include <cmath>
#include "Util.h"

enum class Waveforms
{
	Sine,
	NumWaveforms
};

class Oscillator
{
public:
	double Next(double dt, double frequency, Waveforms waveform);

private:
	double phase = 0.0;
};

