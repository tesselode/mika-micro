#include "Filter.h"

double Filter::Process(double input, double dt, double cutoff, double resonance, double drive)
{
	double f = cutoff * 2.205 * dt;
	f = f > 1 ? 1 : f;
	double high = input - (low + band * (1 - resonance));
	band += f * high;
	low += f * band;
	low = atan(low * drive) / drive;
	return low;
}