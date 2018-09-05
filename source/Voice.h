#pragma once

#include <array>
#include "Envelope.h"
#include "Oscillator.h"
#include "Parameter.h"
#include "Util.h"

class Voice
{
public:
	Voice(std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> &p) : parameters(p) {}
	int GetNote() { return note; }
	void SetNote(int n) {
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Start() { volEnv.Start(); }
	void Release() { volEnv.Release(); }
	double Next(double dt);
	bool IsReleased() { return volEnv.IsReleased(); }
	double GetVolume() { return volEnv.Get(); }

private:
	void UpdateEnvelopes(double dt);
	double GetOscillators(double dt);

	std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> &parameters;
	int note = 0;
	double baseFrequency = 440.0;
	Oscillator osc1a;
	Oscillator osc1b;
	Envelope volEnv;
};

