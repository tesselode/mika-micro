#pragma once

#include "Envelope.h"
#include <math.h>
#include "Oscillator.h"

class Voice
{
public:
	int GetNote();
	void SetNote(int n);

	void Start();
	void Release();
	double GetVolume();
	bool IsReleased();

	double Next(double dt);

private:
	double PitchFactor(double p);
	double PitchToFrequency(double p);

	int note = 69;

	Oscillator osc1;

	Envelope volumeEnvelope;
};

