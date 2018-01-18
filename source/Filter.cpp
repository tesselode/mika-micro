#include "Filter.h"

double Filter::Process(double input, double cutoff)
{
	double smoothing = resonance < 0.0 ? -resonance : 0.0;
	double res = resonance < 0.0 ? 0.0 : resonance;
	cutoff = cutoff < 20.0 ? 20.0 : cutoff > 20000.0 ? 20000.0 : cutoff;
	cutoff *= cutoffMultiplier;

	f += (cutoff - f) * .01; // cutoff smoothing
	double fSquare = f * f;
	velocity += fSquare * (input - (value + velocity * (1 - res)));
	if (smoothing > 0.0) velocity -= velocity * smoothing;
	value += velocity;
	value = atan(value * .5) * 2;
	return value * (1 + smoothing);
}
