#pragma once

#include "Envelope.h"
#include "Oscillator.h"
#include "Util.h"

struct Voice
{
	Envelope volEnv;
	int note = 0;
	double frequency = 0.0;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	void Start(int newNote, bool outOfPhase)
	{
		note = newNote;
		frequency = pitchToFrequency(note);
		if (volEnv.stage == EnvelopeStages::Idle)
		{
			oscFm.phase = 0.0;
			osc1a.phase = 0.0;
			osc1b.phase = outOfPhase ? .33 : 0.0;
			osc2a.phase = 0.0;
			osc2b.phase = outOfPhase ? .33 : 0.0;
		}
		volEnv.stage = EnvelopeStages::Attack;
	}
};