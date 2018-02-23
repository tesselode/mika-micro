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
	void SetFmCoarse(int c) { fmCoarse = c; }
	void SetFmFine(double f) { fmFine = f; }
	void SetFilterF(double f) { filterF = f; }
	void SetFilterResonance(double r) { filterResonance = r; }
	void SetVolumeEnvelopeAttack(double a) { volumeEnvelope.SetAttack(a); };
	void SetVolumeEnvelopeDecay(double d) { volumeEnvelope.SetDecay(d); };
	void SetVolumeEnvelopeSustain(double s) { volumeEnvelope.SetSustain(s); }
	void SetVolumeEnvelopeRelease(double r) { volumeEnvelope.SetRelease(r); }

	double GetVolume() { return volumeEnvelope.Get(); }
	bool IsReleased() { return volumeEnvelope.IsReleased(); }
	int GetNote() { return note; }
	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Start()
	{
		volumeEnvelope.Start();
	}
	void Release()
	{
		volumeEnvelope.Release();
	}
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

	Filter filter;

	double osc1TuneFactor = 1.0;
	double osc2TuneFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double filterF = 1.0;
	double filterResonance = 1.0;

	int note = 69;
	double baseFrequency = 440.0;

	double GetOscillators();
	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
	}
};

