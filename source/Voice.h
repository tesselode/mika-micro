#pragma once

#include "Envelope.h"
#include "Oscillator.h"

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		sampleRate = sr;
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
	}

	int GetNote() { return note; }
	double GetVolume() { return modEnvelope.Get(); }
	bool IsReleased() { return modEnvelope.IsReleased(); }

	void SetNote(int n);

	void SetOscillator1Split(double s) { oscillator1Split = s; }
	void SetOscillatorMix(double m) { oscillatorMix = m; }
	void SetEnvelopeAttack(double a) { modEnvelope.SetAttack(a); }
	void SetEnvelopeDecay(double d) { modEnvelope.SetDecay(d); }
	void SetEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }

	void Start() { modEnvelope.Start(); }
	void Release() { modEnvelope.Release(); }

	double Next();

private:
	double PitchToFrequency(double p) { return 440.0 * pow(1.05946309436, p - 69); }

	double sampleRate = 44100;
	int note = 69;

	double oscillator1Split = 1.0;
	double oscillatorMix = 0.0;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope modEnvelope;
};
