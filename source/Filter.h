#pragma once

#include <cmath>
#include "Util.h"

class Filter
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }

	void SetResonance(double r) { resonance = r; }

	bool IsSilent() { return low == 0.0; }
	void ResetF() { reset = true; }
	double Process(double input, double cutoff);

private:
	double pi = 2 * acos(0.0);

	double dt = 0.0;

	double cutoff = 8000.0;
	bool reset = false;
	double resonance = 0.0;
	double low = 0.0;
	double band = 0.0;
};
