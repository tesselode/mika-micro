#pragma once

#include <cmath>

enum EWaveforms
{
	kWaveformSine,
	kWaveformTriangle,
	kWaveformSaw,
	kWaveformSquare,
	kWaveformPulse,
	kWaveformNoise,
	kNumWaveforms,
	kWaveformNone
};

class Oscillator
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }

	void Reset(double p = 0.0) { phase = p; }
	double Next(double frequency, EWaveforms waveform);

private:
	double twoPi = 4 * acos(0.0);

	double dt = 0.0;

	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;

	double Blep(double phase);
	double GeneratePulse(double width);
};

