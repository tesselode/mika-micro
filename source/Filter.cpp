#include "Filter.h"

double Filter::Process(double input, double cutoff)
{
	double smoothing = resonance < 0.0 ? -resonance : 0.0;
	double res = resonance < 0.0 ? 0.0 : resonance;
	cutoff *= cutoffMultiplier;

	f += (cutoff - f) * .01; // cutoff smoothing
	double fClamped = f > 1.0 ? 1.0 : f < -1.0 ? -1.0 : f > 0.0 ? f * f : f < 0.0 ? -(f * f) : 0.0; // clamp and square
	velocity += fClamped * (input - (value + velocity * (1 - res)));
	if (smoothing > 0.0) velocity -= velocity * smoothing;
	value += velocity;
	value = atan(value * .5) * 2;
	return value * (1 + smoothing);
}
