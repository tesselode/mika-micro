#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	targetF = targetF > 1.0 ? 1.0 : targetF < .001 ? .001 : targetF;
	if (reset)
		f = targetF;
	else
		f = lerp(f, targetF, .001 * dt);

	double high = input - (low + band * (1 - res1));
	band += f * high * dt;
	low += band * dt * (f * (1 - res2) + res2);
	low = FastAtan(low * .1) * 10.0;
	return low;
}
