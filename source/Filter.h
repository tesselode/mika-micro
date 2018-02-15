#pragma once

#include <math.h>
#include "Util.h"

class Filter
{
public:
	double Process(double input, double dt, double resonance, double drive);
	void UpdateF(double dt, double cutoff);

private:
	double GetTargetF(double dt, double cutoff);

	double f = 1.0;
	double band = 0.0;
	double low = 0.0;
};

