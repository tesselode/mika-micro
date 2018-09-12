#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Util.h"

enum class VoiceModes
{
	Poly,
	Mono,
	Legato,
	NumVoiceModes
};

struct Voice
{
	Envelope volEnv;
	Envelope modEnv;
	Envelope lfoEnv;
	int note = 0;
	double targetFrequency = 0.0;
	double frequency = 0.0;
	double velocity = 0.0;
	double pitchBend = 1.0;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Filter filter;

	bool IsReleased() { return volEnv.IsReleased(); }
	double GetVolume() { return volEnv.value; }

	void Reset(bool osc1OutOfPhase, bool osc2OutOfPhase)
	{
		oscFm.phase = 0.0;
		osc1a.phase = 0.0;
		osc1b.phase = osc1OutOfPhase ? .33 : 0.0;
		osc2a.phase = 0.0;
		osc2b.phase = osc2OutOfPhase ? .33 : 0.0;
		volEnv.Reset();
		modEnv.Reset();
		lfoEnv.Reset();
		filter.Reset();
	}

	void Release()
	{
		volEnv.Release();
		modEnv.Release();
		lfoEnv.Release();
	}

	void SetNote(int n)
	{
		note = n;
		targetFrequency = pitchToFrequency(note);
	}

	void SetPitchBendFactor(double f) { pitchBend = f; }

	void ResetPitch() { frequency = targetFrequency; }

	void SetVelocity(double v) { velocity = v; }

	void Start(bool osc1OutOfPhase, bool osc2OutOfPhase)
	{
		if (volEnv.stage == EnvelopeStages::Idle)
			Reset(osc1OutOfPhase, osc2OutOfPhase);
		volEnv.Start();
		modEnv.Start();
		lfoEnv.Start();
	}
};