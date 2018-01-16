#pragma once

#include <math.h>

enum Waveform
{
	Sine,
	Triangle,
	Saw,
	Square,
	Pulse,
	numWaveforms
};

class Oscillator
{
public:
	void SetFrequency(double f) { frequency = f; }
	void SetSampleRate(double sr) { sampleRate = sr; }

	void Update();
	void ResetPhase() { t = 0.0; }
	double Get(Waveform waveform);

private:
	double twoPi = 4 * acos(0.0);

	double Blep(double t);
	double GeneratePulse(double width);

	double frequency = 440.0;
	double sampleRate = 44100.0;

	double t = 0.0;
	double dt = 0.0;
	double inverseDt = 0.0;

	double triCurrent = 0.0;
	double triLast = 0.0;
};

