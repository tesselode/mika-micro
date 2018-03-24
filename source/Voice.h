#pragma once

#include <cmath>
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
		dt = 1.0 / sr;
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

	void SetOsc1Wave(EWaveforms w)
	{
		if (GetVolume() == 0.0)
			osc1Wave = w;
		else
			osc1WaveNext = w;
	}
	void SetOsc1Tune(double p) { osc1TuneFactor = pitchFactor(p); }
	void SetOsc2Wave(EWaveforms w)
	{
		if (GetVolume() == 0.0)
			osc2Wave = w;
		else
			osc2WaveNext = w;
	}
	void SetOsc2Tune(double p) { osc2TuneFactor = pitchFactor(p); }
	void SetOsc1Split(double s)
	{
		osc1SplitEnabledNext = s != 0.0;
		osc1SplitFactorA = 1.0 + s;
		osc1SplitFactorB = 1.0 / osc1SplitFactorA;
	};
	void SetOsc2Split(double s)
	{
		osc2SplitEnabledNext = s != 0.0;
		osc2SplitFactorA = 1.0 + s;
		osc2SplitFactorB = 1.0 / osc2SplitFactorA;
	};
	void SetOscMix(double m) { targetOscMix = m; }
	void SetFmMode(int m) { fmMode = m; }
	void SetFmCoarse(int c) { fmCoarse = c; }
	void SetFmFine(double f) { fmFine = f; }
	void SetFilterEnabled(double enabled)
	{
		filterEnabledNext = enabled;
		if (GetVolume() == 0.0) filterEnabled = enabled;
	}
	void SetFilterCutoff(double c) { filterCutoff = c; }
	void SetFilterResonance(double r) { filter.SetResonance(r); }
	void SetFilterKeyTrack(double t) { filterKeyTrack = t; }
	void SetVolumeEnvelopeAttack(double a) { volumeEnvelope.SetAttack(a); };
	void SetVolumeEnvelopeDecay(double d) { volumeEnvelope.SetDecay(d); };
	void SetVolumeEnvelopeSustain(double s) { volumeEnvelope.SetSustain(s); }
	void SetVolumeEnvelopeRelease(double r) { volumeEnvelope.SetRelease(r); }
	void SetVolumeEnvelopeVelocitySensitivity(double v) { volumeEnvelope.SetVelocitySensitivity(v); }
	void SetModEnvelopeVelocitySensitivity(double v) { modEnvelope.SetVelocitySensitivity(v); }
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
	void SetGlideSpeed(double s) { glideSpeed = s; }

	double GetVolume() { return volumeEnvelope.Get(); }
	bool IsReleased() { return volumeEnvelope.IsReleased(); }
	int GetNote() { return note; }
	void SetNote(int n)
	{
		note = n;
		targetFrequency = pitchToFrequency(note);
	}
	void SetVelocity(double v)
	{
		volumeEnvelope.SetVelocity(v);
		modEnvelope.SetVelocity(v);
	}
	void SetPitchBendFactor(double f) { pitchBendFactor = f; }
	void ResetPitch() { baseFrequency = targetFrequency; }
	void Start();
	void Release();
	double Next(double lfoValue, double driftValue);

private:
	double pi = 2 * acos(0.0);

	double dt = 0.0;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Oscillator oscFm;

	Envelope volumeEnvelope;
	Envelope modEnvelope;
	Envelope delayEnvelope;

	Filter filter;

	EWaveforms osc1Wave = kWaveformSaw;
	EWaveforms osc1WaveNext = kWaveformNone;
	EWaveforms osc2Wave = kWaveformSaw;
	EWaveforms osc2WaveNext = kWaveformNone;
	double osc1TuneFactor = 1.0;
	double osc2TuneFactor = 1.0;
	bool osc1SplitEnabled = false;
	bool osc1SplitEnabledNext = false;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	bool osc2SplitEnabled = false;
	bool osc2SplitEnabledNext = false;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double targetOscMix = 0.0;
	double oscMix = 0.0;
	int fmMode = 0;
	int fmCoarse = 0;
	double fmFine = 0.0;
	bool filterEnabled = false;
	bool filterEnabledNext = false;
	double filterCutoff = 1.0;
	double filterKeyTrack = 0.0;
	double lfoAmount = 0.0;
	double volEnvFm = 0.0;
	double modEnvFm = 0.0;
	double lfoFm = 0.0;
	double volEnvCutoff = 0.0;
	double modEnvCutoff = 0.0;
	double lfoCutoff = 0.0;
	double glideSpeed = 1000.0;

	int note = 69;
	double targetFrequency = 440.0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	double fadeVolume = 1.0;

	double GetFilterCutoff(double lfoValue, double driftValue);
	double GetOscillators(double lfoValue, double driftValue);
	void UpdateSplitAndWave();
	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
		modEnvelope.Update();
		delayEnvelope.Update();
	}
};

