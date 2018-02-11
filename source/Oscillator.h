#pragma once

enum OscillatorWaveform
{
	OscillatorWaveformSaw,
	numWaveforms,
};

class Oscillator
{
public:
	double Next(double dt, double frequency, OscillatorWaveform waveform);

private:
	double Blep(double t, double dt);
	double phase = 0.0;
};