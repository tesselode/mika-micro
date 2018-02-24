#pragma once

#include "Util.h"

class Filter
{
public:
	void SetSampleRate(double sr) { dt = 44100.0 / sr; }

	void SetRes1(double r) { res1 = r; }
	void SetRes2(double r) { res2 = r; }

	double Process(double input, double targetF);

private:
	double FastAtan(double x) { return x / (1.0 + .28 * (x * x)); }

	double dt = 0.0;

	double res1 = 1.0;
	double res2 = 1.0;

	double f = 0.0;
	double low = 0.0;
	double band = 0.0;
};

