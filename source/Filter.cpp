#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	targetF = targetF > 1.0 ? 1.0 : targetF < .001 ? .001 : targetF;
	f = reset ? targetF : lerp(f, targetF, .001 * dt);
	double fSquared = f * f;

	double high = input - (low + band * (1 - res));
	band += fSquared * high * dt;
	low += fSquared * band * dt;
	low = FastAtan(low * .1) * 10.0;
	return low;
}
