#pragma once

#include <array>
#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "Util.h"

class Voice
{
public:
	Voice(std::array<double, kNumParameters> &parameters) : p(parameters) {}

	void SetOsc1Wave(EWaveforms waveform)
	{
		osc1a.SetWaveform(waveform);
		osc1b.SetWaveform(waveform);
	}
	void SetOsc1PitchFactor(double f) { osc1PitchFactor = f; }
	void SetOsc1Split(double s)
	{
		osc1SplitFactorA = 1.0 + s;
		osc1SplitFactorB = 1.0 / osc1SplitFactorA;
	}
	void SetOsc2Wave(EWaveforms waveform)
	{
		osc2a.SetWaveform(waveform);
		osc2b.SetWaveform(waveform);
	}
	void SetOsc2PitchFactor(double f) { osc2PitchFactor = f; }
	void SetOsc2Split(double s)
	{
		osc2SplitFactorA = 1.0 + s;
		osc2SplitFactorB = 1.0 / osc2SplitFactorA;
	}

	void SetNote(int note);
	int GetNote() { return note; }
	void SetVelocity(double v) { velocity = v; }
	void SetPitchBendFactor(double f) { pitchBendFactor = f; }
	void ResetPitch() { baseFrequency = targetFrequency; }
	double GetVolume() { return volEnv.Get(p[kVolEnvV], velocity); }
	bool IsReleased() { return volEnv.IsReleased(); }
	void Start();
	void Release();
	double Next(double dt, double lfoValue, double driftValue);

private:
	double GetFilterCutoff(double volEnvValue, double modEnvValue, double lfoValue, double driftValue);
	void UpdateEnvelopes(double dt);
	void Reset();

	std::array<double, kNumParameters> &p;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Envelope volEnv;
	Envelope modEnv;
	Envelope lfoEnv;
	Filter filter;
	int note = 69;
	double targetFrequency = 440.0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	double velocity = 0.0;
	double osc1PitchFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc1bMix = 0.0;
	double osc2PitchFactor = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double osc2bMix = 0.0;
	double filterMix = 0.0;
};

