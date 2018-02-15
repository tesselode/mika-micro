#pragma once

#include <math.h>

class Filter
{
public:
	double Process(double input, double dt, double cutoff, double resonance, double drive);

private:
	double band = 0.0;
	double low = 0.0;
};

