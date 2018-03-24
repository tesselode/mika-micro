#include "Filter.h"

double Filter::Process(double input, double targetF)
{
	auto targetCutoff = targetF * 20000.0;
	targetCutoff = targetCutoff > 20000.0 ? 20000.0 : targetCutoff < 20.0 ? 20.0 : targetCutoff;
	cutoff = reset ? targetCutoff : lerp(cutoff, targetCutoff, 20.0 * dt);
	auto f = 1 * sin(pi * cutoff * dt);
	f *= f;
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;
	auto maxResonance = 1.0 - f * f * f * f * f;
	auto r = resonance > maxResonance ? maxResonance : resonance;

	for (int i = 0; i < 2; i++)
	{
		auto high = input - (low + band * (1 - r));
		band += f * high;
		low += f * band;
		low = fastAtan(low * .1) * 10.0;
		input = low;
	}

	return input;
}
