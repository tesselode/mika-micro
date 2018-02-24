#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Util.h"

class Voice
{
public:
	Voice() : delayEnvelope(1000.0, 0.5, 1.0, 0.5) {};

	void SetSampleRate(double sr)
	{
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
		oscFm.SetSampleRate(sr);
		volumeEnvelope.SetSampleRate(sr);
		modEnvelope.SetSampleRate(sr);
		delayEnvelope.SetSampleRate(sr);
		filter.SetSampleRate(sr);
	}

	void SetOsc1Wave(OscillatorWaveform w) { osc1Wave = w; }
	void SetOsc1Tune(double p) { osc1TuneFactor = pitchFactor(p); }
	void SetOsc2Wave(OscillatorWaveform w) { osc2Wave = w; }
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
	void SetFilterKeyTrack(double t) { filterKeyTrack = t; }
	void SetVolumeEnvelopeAttack(double a) { volumeEnvelope.SetAttack(a); };
	void SetVolumeEnvelopeDecay(double d) { volumeEnvelope.SetDecay(d); };
	void SetVolumeEnvelopeSustain(double s) { volumeEnvelope.SetSustain(s); }
	void SetVolumeEnvelopeRelease(double r) { volumeEnvelope.SetRelease(r); }
	void SetModEnvelopeAttack(double a) { modEnvelope.SetAttack(a); };
	void SetModEnvelopeDecay(double d) { modEnvelope.SetDecay(d); };
	void SetModEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetModEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }
	void SetLfoAmount(double a) { lfoAmount = a; }
	void SetLfoDelay(double d) { delayEnvelope.SetAttack(d); }
	void SetVolumeEnvelopeFm(double f) { volEnvFm = f; }
	void SetModEnvelopeFm(double f) { modEnvFm = f; }
	void SetLfoFm(double f) { lfoFm = f; }
	void SetVolumeEnvelopeCutoff(double c) { volEnvCutoff = c; }
	void SetModEnvelopeCutoff(double c) { modEnvCutoff = c; }
	void SetLfoCutoff(double c) { lfoCutoff = c; }

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
	double Next(double lfoValue);

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
	Envelope delayEnvelope;

	Filter filter;

	OscillatorWaveform osc1Wave = OscillatorWaveformSaw;
	OscillatorWaveform osc2Wave = OscillatorWaveformSaw;
	double osc1TuneFactor = 1.0;
	double osc2TuneFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double oscMix = 0.0;
	double filterF = 1.0;
	double filterKeyTrack = 0.0;
	double lfoAmount = 0.0;
	double volEnvFm = 0.0;
	double modEnvFm = 0.0;
	double lfoFm = 0.0;
	double volEnvCutoff = 0.0;
	double modEnvCutoff = 0.0;
	double lfoCutoff = 0.0;

	int note = 69;
	double baseFrequency = 440.0;

	double GetFilterF(double lfoValue);
	double GetOscillators(double lfoValue);
	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
		modEnvelope.Update();
		delayEnvelope.Update();
	}
};

