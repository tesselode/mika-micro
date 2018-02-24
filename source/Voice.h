#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
		oscFm.SetSampleRate(sr);
		volumeEnvelope.SetSampleRate(sr);
		modEnvelope.SetSampleRate(sr);
		filter.SetSampleRate(sr);
	}

	void SetOsc1Tune(double p) { osc1TuneFactor = pitchFactor(p); }
	void SetOsc2Tune(double p) { osc2TuneFactor = pitchFactor(p); }
	void SetOsc1Split(double s)
	{
		osc1SplitFactorA = 1.0 + s;
		osc1SplitFactorB = 1.0 / osc1SplitFactorA;
	};
	void SetOsc2Split(double s)
	{
		osc2SplitFactorA = 1.0 + s;
		osc2SplitFactorB = 1.0 / osc2SplitFactorA;
	};
	void SetOscMix(double m) { oscMix = m; }
	void SetFmCoarse(int c) { fmCoarse = c; }
	void SetFmFine(double f) { fmFine = f; }
	void SetFilterF(double f) { filterF = f; }
	void SetFilterRes1(double r) { filter.SetRes1(r); }
	void SetFilterRes2(double r) { filter.SetRes2(r); }
	void SetVolumeEnvelopeAttack(double a) { volumeEnvelope.SetAttack(a); };
	void SetVolumeEnvelopeDecay(double d) { volumeEnvelope.SetDecay(d); };
	void SetVolumeEnvelopeSustain(double s) { volumeEnvelope.SetSustain(s); }
	void SetVolumeEnvelopeRelease(double r) { volumeEnvelope.SetRelease(r); }
	void SetModEnvelopeAttack(double a) { modEnvelope.SetAttack(a); };
	void SetModEnvelopeDecay(double d) { modEnvelope.SetDecay(d); };
	void SetModEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetModEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }
	void SetVolumeEnvelopeFm(double f) { volEnvFm = f; }
	void SetModEnvelopeFm(double f) { modEnvFm = f; }
	void SetVolumeEnvelopeCutoff(double c) { volEnvCutoff = c; }
	void SetModEnvelopeCutoff(double c) { modEnvCutoff = c; }

	double GetVolume() { return volumeEnvelope.Get(); }
	bool IsReleased() { return volumeEnvelope.IsReleased(); }
	int GetNote() { return note; }
	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Start();
	void Release();
	double Next();

private:
	int fmCoarse = 0;
	double fmFine = 0.0;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Oscillator oscFm;

	Envelope volumeEnvelope;
	Envelope modEnvelope;

	Filter filter;

	double osc1TuneFactor = 1.0;
	double osc2TuneFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double oscMix = 0.0;
	double filterF = 1.0;
	double volEnvFm = 0.0;
	double modEnvFm = 0.0;
	double volEnvCutoff = 0.0;
	double modEnvCutoff = 0.0;

	int note = 69;
	double baseFrequency = 440.0;

	double GetFilterF();
	double GetOscillators();
	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
		modEnvelope.Update();
	}
};

