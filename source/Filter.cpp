#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	targetF = targetF > 1.0 ? 1.0 : targetF < .001 ? .001 : targetF;
	f = lerp(f, targetF, .01);

	double high = input - (low + band * resonance);
	band += f * high * dt;
	low += f * band * dt;
	low = FastAtan(low * .1) * 10.0;
	return low;
}
