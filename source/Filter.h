#pragma once

#include <math.h>

class Filter
{
public:
	void SetSampleRate(double sr) { sampleRate = sr; }
	void SetSmoothing(double r) { smoothing = r; }

	double Process(double input, double cutoff);

private:
	double sampleRate = 44100.0;
	double f = 1.0;
	double smoothing = 0.0;

	double velocity = 0.0;
	double value = 0.0;
};

