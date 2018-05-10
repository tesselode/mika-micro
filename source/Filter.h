#pragma once

#include <cmath>
#include "Util.h"

class Filter
{
public:
	bool IsSilent() { return low == 0.0; }
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double band = 0.0;
	double low = 0.0;
};

