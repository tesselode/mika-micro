#pragma once

#include <math.h>
#include "Oscillator.h"

class Voice
{
public:
	int GetNote();
	void SetNote(int n);

	double Next(double dt);

private:
	double PitchFactor(double p);
	double PitchToFrequency(double p);

	int note = 69;

	Oscillator osc1;
};

