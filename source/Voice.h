#pragma once

#include "Envelope.h"
#include "Filter.h"
#include <math.h>
#include "Oscillator.h"
#include <stdlib.h>

class Voice
{
public:
	Voice() : gateEnvelope(100., 1., 1., 50.), lfoDelayEnvelope(100.0, 1.0, 1.0, 1.0) {};

	void SetSampleRate(double sr)
	{
		sampleRate = sr;
		dt = 1.0 / sr;
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
		filter.SetSampleRate(sr);
		modEnvelope.SetSampleRate(sr);
		gateEnvelope.SetSampleRate(sr);
		lfoDelayEnvelope.SetSampleRate(sr);
	}

	int GetNote() { return note; }
	double GetVolume() {

		return (1 - volumeEnvelopeAmount) * gateEnvelope.Get() + volumeEnvelopeAmount * modEnvelope.Get();
	}
	bool IsReleased() { return modEnvelope.IsReleased(); }

	void SetNote(int n);
	void SetPitchBend(double p) { pitchBend = PitchFactor(p * 2); }
	void SetEnvelopeVelocityAmount(double a) { modEnvelope.SetVelocityAmount(a); }
	void SetVelocity(double v) { modEnvelope.SetVelocity(v); }

	void SetOscillator1Wave(Waveform w) { oscillator1Wave = w; }
	void SetOscillator1Split(double s) { oscillator1Split = s; }
	void SetOscillator1Coarse(double c) { oscillator1Coarse = PitchFactor(c); }
	void SetOscillator2Wave(Waveform w) { oscillator2Wave = w; }
	void SetOscillator2Split(double s) { oscillator2Split = s; }
	void SetOscillator2Coarse(double c) { oscillator2Coarse = PitchFactor(c); }
	void SetOscillatorMix(double m) { oscillatorMix = m; }
	void SetFmCoarse(int c) { fmCoarse = c; }
	void SetFmFine(double f) { fmFine = f; }
	void SetFmEnvelopeAmount(double a) { fmEnvelopeAmount = a; }
	void SetFilterCutoff(double c) { filterCutoff = c; }
	void SetFilterResonance(double r) { filter.SetResonance(r); }
	void SetFilterKeyTracking(double k) { filterKeyTracking = k; }
	void SetFilterEnvelopeAmount(double a) { filterEnvelopeAmount = a; }
	void SetEnvelopeAttack(double a) { modEnvelope.SetAttack(a); }
	void SetEnvelopeDecay(double d) { modEnvelope.SetDecay(d); }
	void SetEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }
	void SetLfoAmount(double a) { lfoAmount = a; }
	void SetLfoDelay(double d) { lfoDelayEnvelope.SetAttack(d); }
	void SetMono(bool m) { mono = m; }
	void SetGlideSpeed(double s) { glideSpeed = s; }
	void SetVolumeEnvelopeAmount(double a) { volumeEnvelopeAmount = a; }

	void Start()
	{
		if (IsReleased())
		{
			frequency = targetFrequency;
		}
		if (GetVolume() == 0.0)
		{
			osc1a.ResetPhase();
			osc1b.ResetPhase();
			osc2a.ResetPhase();
			osc2b.ResetPhase();
		}
		modEnvelope.Start();
		gateEnvelope.Start();
		lfoDelayEnvelope.Reset();
		lfoDelayEnvelope.Start();
	}
	void Release()
	{
		modEnvelope.Release();
		gateEnvelope.Release();
	}
	void Reset()
	{
		modEnvelope.Reset();
		gateEnvelope.Reset();
		lfoDelayEnvelope.Reset();
	}

	double Next(double lfoValue);

private:
	// https://stackoverflow.com/a/17798317
	double randomMultiplier = 1.0 / (RAND_MAX * .5);
	double randomValue() { return rand() * randomMultiplier - 1.0; }

	double PitchFactor(double p) { return pow(1.0595, p); } // slight octave stretch
	double PitchToFrequency(double p) { return 440.0 * PitchFactor(p - 69); }

	double sampleRate = 44100.0;
	double dt = 1.0 / 44100.0;
	int note = 69;
	double pitchBend = 1.0;
	double targetFrequency = 440.0;
	double frequency = 440.0;
	double driftPhase = 0.0;

	Waveform oscillator1Wave = Saw;
	double oscillator1Split = 1.0;
	double oscillator1Coarse = 1.0;
	Waveform oscillator2Wave = Saw;
	double oscillator2Split = 1.0;
	double oscillator2Coarse = 1.0;
	double oscillatorMix = 0.0;
	int fmCoarse = 0;
	double fmFine = 0.0;
	double fmEnvelopeAmount = 0.0;
	double filterCutoff = 20000.0;
	double filterKeyTracking = 0.0;
	double filterEnvelopeAmount = 0.0;
	double lfoAmount = 0.0;
	bool mono = false;
	double glideSpeed = 1000.;
	double volumeEnvelopeAmount = 0.0;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope modEnvelope;
	Envelope gateEnvelope;
	Envelope lfoDelayEnvelope;

	Filter filter;
};
