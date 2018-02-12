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

	double GetOsc1Frequency(std::vector<double> &parameters);
	double GetOsc2Frequency(std::vector<double> &parameters);
	double GetOsc1(double dt, std::vector<double> &parameters);
	double GetOsc2(double dt, std::vector<double> &parameters);
	double GetOscillators(double dt, std::vector<double> &parameters);

	int note = 69;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope volumeEnvelope;
};

