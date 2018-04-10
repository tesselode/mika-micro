#include "Filter.h"

double Filter::Process(double dt, double input, double cutoff, double resonance)
{
	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

	// resonance rolloff
	auto maxResonance = 1.0 - f * f * f * f * f;
	resonance = resonance > maxResonance ? maxResonance : resonance;

	// main processing
	auto high = input - (low + band * (1.0 - resonance));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10.0;

	return low;
}
