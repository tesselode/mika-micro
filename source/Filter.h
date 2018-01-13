#pragma once

#include <math.h>

class Filter
{
public:
	Filter()
	{
		SetResonance(1.);
	}

	void SetSampleRate(double sr) { sampleRate = sr; }
	void SetResonance(double r)
	{
		q = r;
		scale = sqrt(q);
	}

	double Process(double input, double cutoff);

private:
	double sampleRate = 44100.0;

	double f;
	double q;
	double scale;

	double low = 0.0;
	double band = 0.0;
};

