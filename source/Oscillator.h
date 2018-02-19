#pragma once

#include <math.h>

enum OscillatorWaveform
{
	OscillatorWaveformSine,
	OscillatorWaveformTriangle,
	OscillatorWaveformSaw,
	OscillatorWaveformSquare,
	OscillatorWaveformPulse,
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
	double GeneratePulse(double width, double phaseIncrement);
	double phase = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
};