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

// https://stackoverflow.com/a/1640399
unsigned long xorshf96()
{
	static unsigned long x = 123456789, y = 362436069, z = 521288629;

	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

// http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
double fastSin(double x)
{
	if (x < -3.14159265)
		x += 6.28318531;
	else if (x > 3.14159265)
		x -= 6.28318531;
	if (x < 0)
		return 1.27323954 * x + .405284735 * x * x;
	else
		return 1.27323954 * x - .405284735 * x * x;
}
