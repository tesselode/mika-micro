#pragma once

#include "Envelope.h"
#include "Filter.h"
#include <math.h>
#include "Oscillator.h"
#include "Parameters.h"
#include "Util.h"
#include <vector>

class Voice
{
public:
	int GetNote();
	void SetNote(int n);
	double GetVolume();
	bool IsReleased();
	void SetPitchBendFactor(double v);

	void SetOsc1Pitch(int coarse, double fine);
	void SetOsc2Pitch(int coarse, double fine);

	void Start();
	void Release();

	double Next(double dt, std::vector<double> &parameters, double lfoValue);

private:
	double GetBaseFrequency();
	double GetFmAmount(std::vector<double> &parameters, double lfoValue);
	double GetOsc1Frequency(std::vector<double> &parameters, double fm, double lfoValue);
	double GetOsc2Frequency(std::vector<double> &parameters, double fm, double lfoValue);
	double GetOscFm(double dt, std::vector<double> &parameters, double lfoValue);
	double GetOsc1(double dt, std::vector<double> &parameters, double fm, double lfoValue);
	double GetOsc2(double dt, std::vector<double> &parameters, double fm, double lfoValue);
	double GetOscillators(double dt, std::vector<double> &parameters, double lfoValue);
	double GetFilterCutoff(std::vector<double> &parameters, double lfoValue);

	void UpdateEnvelopes(double dt, std::vector<double> &parameters);
	double GetLfoAmount(double lfoValue);

	int note = 69;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	double osc1Factor = 1.0;
	double osc2Factor = 1.0;

	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope volumeEnvelope;
	Envelope modEnvelope;
	Envelope delayEnvelope;

	Filter filter;
};

