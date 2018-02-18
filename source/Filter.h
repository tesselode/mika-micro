#pragma once

#include <math.h>
#include "Util.h"

class Filter
{
public:
	double Process(double input, double dt, double res1, double res2);
	void UpdateF(double dt, double cutoff);

private:
	double FastAtan(double x);
	double GetTargetF(double cutoff);

	double f = 1.0;
	double band = 0.0;
	double low = 0.0;
};

