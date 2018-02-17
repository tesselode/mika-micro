#include "Filter.h"

double Filter::FastAtan(double x)
{
	return x / (1.0 + .28 * (x * x));
}

double Filter::GetTargetF(double cutoff)
{
	double f = cutoff * .00005;
	f = f > 1 ? 1 : f < 0 ? 0 : f;
	return f;
}

void Filter::UpdateF(double dt, double cutoff)
{
	f = lerp(f, GetTargetF(cutoff), dt * 100);
}

double Filter::Process(double input, double dt, double resonance, double drive)
{
	dt *= 44100;
	double high = input - (low + band * (1 - resonance));
	band += f * high * dt;
	low += band * dt * (resonance + f * (1 - resonance));
	low = FastAtan(low * drive) / drive;
	return low;
}