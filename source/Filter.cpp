#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	targetF = targetF > 1.0 ? 1.0 : targetF < .001 ? .001 : targetF;
	f = reset ? targetF : lerp(f, targetF, .001 * dt);
	double fSquared = f * f;

	auto maxResonance = 1.0 - fSquared * fSquared * fSquared * fSquared * fSquared;
	auto r = resonance > maxResonance ? maxResonance : resonance;
	double high = input - (low + band * (1 - r));
	band += fSquared * high * dt;
	low += fSquared * band * dt;
	low = FastAtan(low * .1) * 10.0;
	return low;
}
