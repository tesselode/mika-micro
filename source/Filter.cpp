#include "Filter.h"

double Filter::Process(double input, double targetCutoff)
{
	targetCutoff *= .4; // cutoff actually goes a good amount beyond 20khz, so this compensates for that

	// cutoff clamping
	targetCutoff = targetCutoff > 20000.0 ? 20000.0 : targetCutoff < 20.0 ? 20.0 : targetCutoff;

	// cutoff smoothing
	cutoff = reset ? targetCutoff : lerp(cutoff, targetCutoff, 100 * dt);
	reset = false;

	// calculate f
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

	// resonance rolloff
	auto maxResonance = 1.0 - f * f * f * f * f;
	auto r = resonance > maxResonance ? maxResonance : resonance;

	// main processing
	auto high = input - (low + band * (1 - r));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10.0;

	return low;
}
