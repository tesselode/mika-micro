#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	// cutoff clamping
	targetF = targetF > 1.0 ? 1.0 : targetF < .001 ? .001 : targetF;

	// cutoff smoothing
	f = reset ? targetF : lerp(f, targetF, .001 * dt);

	// cutoff curve
	double fSquared = f * f;

	// resonance rolloff
	auto maxResonance = 1.0 - fSquared * fSquared * fSquared * fSquared * fSquared;
	auto r = resonance > maxResonance ? maxResonance : resonance;

	// main processing (2x oversampled)
	double high = input - (low + band * (1 - r));
	band += fSquared * high * dt;
	low += fSquared * band * dt;
	low = fastAtan(low * .1) * 10.0;
	high = low - (low + band * (1 - r));
	band += fSquared * high * dt;
	low += fSquared * band * dt;
	low = fastAtan(low * .1) * 10.0;
	
	return low;
}
