#pragma once

#include "Envelope.h"
#include <math.h>

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		sampleRate = sr;
		modEnvelope.SetSampleRate(sr);
	}

	void SetNote(int n);
	void SetEnvelopeAttack(double a) { modEnvelope.SetAttack(a); }
	void SetEnvelopeDecay(double d) { modEnvelope.SetDecay(d); }
	void SetEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }

	void Start() { modEnvelope.Start(); }
	void Release() { modEnvelope.Release(); }

	double Next();

private:
	double twoPi = 4 * acos(0.0);

	double PitchToFrequency(double p) { return 440.0 * pow(1.05946309436, p - 69); }

	double Sine(double t);

	Envelope modEnvelope;

	double sampleRate = 44100;
	double frequency = 440.0;

	double phase1a = 0.0;
	double phase1b = 0.0;
	double phase2a = 0.0;
	double phase2b = 0.0;
};
