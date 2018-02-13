#include "Filter.h"

double Filter::fastAtan(double x)
{
	return fourPi * x - x * (fabs(x) - 1) * (.2447 + .0663 * fabs(x));
}

double Filter::Process(double input, double dt, double cutoff, double resonance)
{
	double f = cutoff * .00005;
	double high = input - (low + band * (1 - resonance));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10;
	return low;
}