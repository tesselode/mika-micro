#pragma once

#include "Envelope.h"
#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
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
	int note = 0;
	double baseFrequency = 440.0;
	Oscillator osc1a;
	Envelope volEnv;
};

