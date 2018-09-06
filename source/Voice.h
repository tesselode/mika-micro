#pragma once

#include <array>
#include <cmath>
#include "Envelope.h"
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
	void SetNote(int n) {
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Reset();
	void Start();
	void Release() { volEnv.Release(); }
	double Next(double dt);
	bool IsReleased() { return volEnv.IsReleased(); }
	double GetVolume() { return volEnv.Get(); }

private:
	void UpdateEnvelopes(double dt);
	double GetFmMultiplier(double dt);
	double GetOscillator1Frequency(double dt, bool skipFm = false);
	double GetOscillator2Frequency(double dt);
	double GetOscillator1(double dt);
	double GetOscillator2(double dt);
	double GetOscillators(double dt);

	std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> &parameters;
	int note = 0;
	double baseFrequency = 440.0;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Envelope volEnv;
};

