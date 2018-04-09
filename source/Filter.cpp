#include "Filter.h"

double Filter::Process(double dt, double input, double cutoff, double resonance)
{
	cutoff = cutoff > 8000.0 ? 8000.0 : cutoff < 20.0 ? 20.0 : cutoff;
	auto f = 2 * sin(pi * cutoff * dt);
	auto maxResonance = 1.0 - f * f * f * f * f;
	resonance = resonance > maxResonance ? maxResonance : resonance;

	auto high = input - (low + band * (1.0 - resonance));
	band += f * high;
	low += f * high;
	low = fastAtan(low * .1) * 10.0;

	return low;
}
