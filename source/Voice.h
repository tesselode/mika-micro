#pragma once

#include "Envelope.h"
#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
	void SetNote(int n) {
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Start() { volEnv.Start(); }
	void Release() { volEnv.Release(); }
	double Next(double dt);

private:
	int note = 0;
	double baseFrequency = 440.0;
	Oscillator osc1a;
	Envelope volEnv;
};

