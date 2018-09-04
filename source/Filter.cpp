#include "Filter.h"

double TwoPoleFilter::Process(double dt, double input, double cutoff, double resonance)
{
	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > .99 ? .99 : f < .01 ? .01 : f;

	// feedback calculation
	auto feedback = resonance + resonance / (1.0 - f);
	feedback = fastAtan(feedback * .1) * 10.0;

	// main processing
	a += f * (input - a + feedback * (a - b));
	a = fastAtan(a * .1) * 10.0;
	b += f * (a - b);
	b = fastAtan(b * .1) * 10.0;

	return b;
}

double StateVariableFilter::Process(double dt, double input, double cutoff, double resonance)
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

double FourPoleFilter::Process(double dt, double input, double cutoff, double resonance)
{
	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > .99 ? .99 : f < .01 ? .01 : f;

	// feedback calculation
	auto feedback = resonance + resonance / (1.0 - f);
	feedback = fastAtan(feedback * .1) * 10.0;

	// main processing
	a += f * (input - a + feedback * (a - b));
	a = fastAtan(a * .1) * 10.0;
	b += f * (a - b);
	b = fastAtan(b * .1) * 10.0;
	c += f * (b - c);
	c = fastAtan(c * .1) * 10.0;
	d += f * (c - d);
	d = fastAtan(d * .1) * 10.0;

	return d;
}

void MultiFilter::SetMode(FilterModes m)
{
	if (currentMode != m)
	{
		previousMode = currentMode;
		currentMode = m;
		crossfading = true;
		currentModeMix = 0.0;
	}
}

double MultiFilter::Process(double dt, double input, double cutoff, double resonance)
{
	switch (crossfading)
	{
	case true:
	{
		currentModeMix += 100.0 * dt;
		if (currentModeMix >= 1.0)
		{
			auto current = currentMode;
			currentModeMix = 1.0;
			crossfading = false;
		}

		value = filters[(int)previousMode]->Process(dt, input, cutoff, resonance) * (1.0 - currentModeMix);
		value += filters[(int)currentMode]->Process(dt, input, cutoff, resonance) * currentModeMix;
		break;
	}
	case false:
		value = filters[(int)currentMode]->Process(dt, input, cutoff, resonance) * currentModeMix;
		break;
	}

	return value;
}
