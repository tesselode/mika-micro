#pragma once

#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		osc1a.SetSampleRate(sr);
	}

	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(note);
	}

	double Next();

private:
	Oscillator osc1a;

	int note = 69;
	double baseFrequency = 440.0;
};

