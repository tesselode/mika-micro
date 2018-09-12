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

// waveform generation //

// http://www.kvraudio.com/forum/viewtopic.php?t=375517
inline double Blep(double phase, double phaseIncrement)
{
	if (phase < phaseIncrement)
	{
		phase /= phaseIncrement;
		return phase + phase - phase * phase - 1.0;
	}
	else if (phase > 1.0 - phaseIncrement)
	{
		phase = (phase - 1.0) / phaseIncrement;
		return phase * phase + phase + phase + 1.0;
	}
	return 0.0;
}

inline double GeneratePulse(double phase, double phaseIncrement, double width)
{
	double v = phase < width ? 1.0 : -1.0;
	v += Blep(phase, phaseIncrement);
	v -= Blep(fmod(phase + (1.0 - width), 1.0), phaseIncrement);
	return v;
}

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