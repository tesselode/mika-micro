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

double Filter::Process(double input, double dt, double res1, double res2)
{
	dt *= 44100;
	double high = input - (low + band * (1 - res1));
	band += f * high * dt;
	low += band * dt * (res2 + f * (1 - res2));
	low = FastAtan(low * .1) * 10.0;
	return low;
}