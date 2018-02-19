#include "Util.h"

double lerp(double a, double b, double f)
{
	return a + (b - a) * f;
}

double pitchFactor(double p)
{
	return pow(1.0595, p);
}

double pitchToFrequency(double p)
{
	return 440.0 * pitchFactor(p - 69);
}