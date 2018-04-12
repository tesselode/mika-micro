#pragma once

#include <cmath>

// math constants //
const double pi = 2.0 * acos(0.0);
const double twoPi = 2.0 * pi;

// pitch calculation //
inline double pitchFactor(double p) { return pow(1.0595, p); }
inline double pitchToFrequency(double p) { return 440.0 * pitchFactor(p - 69); }

// fast trig //
inline double fastAtan(double x) { return x / (1.0 + .28 * (x * x)); }

// random numbers //

// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
static unsigned long x = 123456789, y = 362436069, z = 521288629;
inline unsigned long xorshift(void)
{
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

const double xorshiftMultiplier = 2.0 / ULONG_MAX;
inline double random()
{
	return -1.0 + xorshift() * xorshiftMultiplier;
}