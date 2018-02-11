#pragma once

#include "Envelope.h"
#include <math.h>
#include "Oscillator.h"
#include "Parameters.h"
#include <vector>

class Voice
{
public:
	int GetNote();
	void SetNote(int n);
	double GetVolume();
	bool IsReleased();

	void Start();
	void Release();

	double Next(double dt, std::vector<double> &parameters);

private:
	double PitchFactor(double p);
	double PitchToFrequency(double p);

	int note = 69;

	Oscillator osc1;

	Envelope volumeEnvelope;
};

