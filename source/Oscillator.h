#pragma once

#include <cmath>

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
	void SetSampleRate(double sr) { dt = 1.0 / sr; }

	void Reset(double p = 0.0) { phase = p; }
	double Next(double frequency, OscillatorWaveform waveform);

private:
	double twoPi = 4 * acos(0.0);

	double dt = 0.0;

	double phase = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;

	double Blep(double t, double dt);
	double GeneratePulse(double width, double phaseIncrement);
};

