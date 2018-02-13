#pragma once

#include <math.h>

class Filter
{
public:
	double Process(double input, double dt, double cutoff, double resonance);

private:
	double fourPi = 8 * acos(0.0);
	double fastAtan(double x);

	double band = 0.0;
	double low = 0.0;
};

