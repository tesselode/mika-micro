#include "Filter.h"

double Filter::Process(double input, double cutoff)
{
	cutoff /= 20000.;
	cutoff *= (44100 / sampleRate);
	f += (cutoff - f) * .01; // cutoff smoothing
	double fClamped = f > 1.0 ? 1.0 : f < -1.0 ? -1.0 : f > 0.0 ? f * f : f < 0.0 ? -(f * f) : 0.0; // clamp and square
	velocity += fClamped * (input - (value + velocity));
	velocity -= velocity * smoothing;
	value += velocity;
	value = atan(value / 2) * 2;
	return value * (1 + smoothing);
}
