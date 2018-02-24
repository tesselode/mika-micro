#pragma once

#include <cmath>

enum OscillatorWaveform
{
	OscillatorWaveformSine,
	OscillatorWaveformSaw,
	numWaveforms
};

class Oscillator
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }

	void Reset() { phase = 0.0; }
	double Next(double frequency, OscillatorWaveform waveform);

private:
	double twoPi = 4 * acos(0.0);

	double dt = 0.0;

	double phase = 0.0;

	double Blep(double t, double dt);
};

