#pragma once

#include "Envelope.h"
#include "Filter.h"
#include <math.h>
#include "Oscillator.h"
#include "Parameters.h"
#include <random>
#include "Util.h"
#include <vector>

class Voice
{
public:
	Voice(std::vector<double>& params, int seed);

	int GetNote();
	void SetNote(int n);
	void SetVelocity(int v);
	double GetVolume();
	bool IsReleased();
	void SetPitchBendFactor(double v);

	void SetOsc1Pitch(int coarse, double fine);
	void SetOsc2Pitch(int coarse, double fine);

	void Start();
	void Release();

	double Next(double dt, double lfo);

private:
	std::vector<double>& parameters;
	int note = 69;
	int velocity = 0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	double osc1Factor = 1.0;
	double osc2Factor = 1.0;
	double deltaTime = 0.0;
	double fmValue = 0.0;
	double lfoValue = 0.0;
	double driftValue = 0.0;

	std::mt19937 gen;
	std::uniform_real_distribution<> dist;
	double driftPhase = 0.0;

	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope volumeEnvelope;
	Envelope modEnvelope;
	Envelope delayEnvelope;

	Filter filter;

	double GetBaseFrequency();
	double GetLfoAmount();
	double GetFmAmount();
	double GetOsc1Frequency(bool applyFm);
	double GetOsc2Frequency(bool applyFm);
	double GetOscFm();
	double GetOsc1();
	double GetOsc2();
	double GetOscillators();
	double GetFilterCutoff();
	void UpdateDrift();
	void UpdateEnvelopes();
	void Reset();
};

