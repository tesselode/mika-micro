#pragma once

#include <cmath>
#include "Util.h"

class Filter
{
public:
	void Reset() { reset = true; }
	bool IsSilent() { return low == 0.0; }
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double cutoff = 8000.0;
	bool reset = false;
	double band = 0.0;
	double low = 0.0;
};

