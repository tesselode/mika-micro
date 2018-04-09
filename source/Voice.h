#pragma once

#include "Envelope.h"
#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
	void SetNote(int note);
	int GetNote() { return note; }
	double GetVolume() { return volEnv.Get(); }
	bool IsReleased() { return volEnv.IsReleased(); }
	void Start();
	void Release();
	double Next(double dt);

private:
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Envelope volEnv;
	Envelope modEnv;
	Envelope lfoEnv;
	int note = 69;
	double baseFrequency = 440.0;
};

