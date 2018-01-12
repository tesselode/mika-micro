#pragma once

#include <math.h>

class Voice
{
public:
	void SetSampleRate(double sr) { sampleRate = sr; }
	void SetNote(int n);
	double Next();

private:
	double twoPi = 4 * acos(0.0);

	double PitchToFrequency(double p) { return 440.0 * pow(1.05946309436, p - 69); }

	double Sine(double t);

	double sampleRate = 44100;
	double frequency = 440.0;

	double phase1a = 0.0;
	double phase1b = 0.0;
	double phase2a = 0.0;
	double phase2b = 0.0;
};

