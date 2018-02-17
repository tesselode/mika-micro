#pragma once

#include <math.h>

enum OscillatorWaveform
{
	OscillatorWaveformSine,
	OscillatorWaveformSaw,
	numWaveforms,
};

class Oscillator
{
public:
	void ResetPhase();
	double Next(double dt, double frequency, OscillatorWaveform waveform);

private:
	double twoPi = 4 * acos(0.0);

	double Blep(double t, double dt);
	double phase = 0.0;
};