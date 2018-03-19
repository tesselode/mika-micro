#pragma once

#include "Util.h"

class Filter
{
public:
	void SetSampleRate(double sr) { dt = 44100.0 / sr; }

	void SetResonance(double r) { resonance = r; }

	bool IsSilent() { return low == 0.0; }

	void ResetF() { reset = true; }
	double Process(double input, double targetF);

private:
	double dt = 0.0;

	double resonance = 1.0;

	bool reset = false;
	double f = 1.0;
	double low = 0.0;
	double band = 0.0;
};

