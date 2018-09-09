#pragma once

#include <array>
#include <cmath>
#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Parameter.h"
#include "Util.h"

enum class FmModes
{
	Off,
	Osc1,
	Osc2,
	NumFmModes
};

class Voice
{
public:
	Voice(std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> &p) : parameters(p) {}
	int GetNote() { return note; }
	void SetNote(int n);
	void SetVelocity(double v) { velocity = v; }
	void SetPitchBendFactor(double f) { pitchBendFactor = f; }
	void ResetPitch() { baseFrequency = targetFrequency; }
	void Reset();
	void Start();
	void Release();
	double Next(double dt, double lfoValue, double driftValue);
	bool IsReleased() { return volEnv.IsReleased(); }
	double GetVolume() { return volEnv.Get(); }

private:
	void UpdateEnvelopes(double dt);
	void UpdateGlide(double dt);
	double GetFmMultiplier(double dt, double lfoValue, double driftValue);
	double GetOscillator1Frequency(double dt, double lfoValue, double driftValue, bool skipFm = false);
	double GetOscillator2Frequency(double dt, double lfoValue, double driftValue);
	double GetOscillator1(double dt, double lfoValue, double driftValue);
	double GetOscillator2(double dt, double lfoValue, double driftValue);
	double GetOscillators(double dt, double lfoValue, double driftValue);

	double ApplyFilter(double dt, double input, double lfoValue, double driftValue);

	std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> &parameters;
	int note = 0;
	double velocity = 0.0;
	double targetFrequency = 440.0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Envelope volEnv;
	Envelope modEnv;
	Envelope lfoEnv;
	TwoPoleFilter twoPole;
	StateVariableFilter svf;
	FourPoleFilter fourPole;
};

