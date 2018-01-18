#pragma once

#include <math.h>

class Filter
{
public:
	// accomodate for sample rate and convert from frequency to 0.0 - 1.0
	void SetSampleRate(double sr) { cutoffMultiplier = 44100.0 / sr / 20000.0; }
	void SetResonance(double r) { resonance = r; }

	double Process(double input, double cutoff);

private:
	double cutoffMultiplier = 0.0;
	double f = 1.0;
	double resonance = 0.0;

	double velocity = 0.0;
	double value = 0.0;
};

