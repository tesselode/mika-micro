#pragma once

#include "Util.h"

class Filter
{
public:
	void SetSampleRate(double sr) { dt = 44100.0 / sr; }

	void SetResonance(double r) { resonance = r; }

	double Process(double input, double targetF);

private:
	double FastAtan(double x) { return x / (1.0 + .28 * (x * x)); }

	double dt = 0.0;

	double resonance = 0.0;

	double f = 0.0;
	double low = 0.0;
	double band = 0.0;
};

