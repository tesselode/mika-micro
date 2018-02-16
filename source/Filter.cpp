#include "Filter.h"

double Filter::GetTargetF(double dt, double cutoff)
{
	double f = cutoff * 2.205 * dt;
	f = f > 1 ? 1 : f < 0 ? 0 : f;
	return f;
}

void Filter::UpdateF(double dt, double cutoff)
{
	f = lerp(f, GetTargetF(dt, cutoff), dt * 10);
}

double Filter::Process(double input, double dt, double resonance, double drive)
{
	double high = input - (low + band * (1 - resonance));
	band += f * high;
	low += band * f * (1 - resonance) + band * resonance;
	low = atan(low * drive) / drive;
	return low;
}