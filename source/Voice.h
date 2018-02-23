#pragma once

#include "Envelope.h"
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
	}

	void SetOsc1Split(double s)
	{
		osc1SplitFactorA = 1.0 + s;
		osc1SplitFactorB = 1.0 / osc1SplitFactorA;
		CalculateFrequencies();
	};
	void SetOsc2Split(double s)
	{
		osc2SplitFactorA = 1.0 + s;
		osc2SplitFactorB = 1.0 / osc2SplitFactorA;
		CalculateFrequencies();
	};
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
		CalculateFrequencies();
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
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Oscillator oscFm;

	Envelope volumeEnvelope;

	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double osc1aFrequency = 440.0;
	double osc1bFrequency = 440.0;
	double osc2aFrequency = 440.0;
	double osc2bFrequency = 440.0;

	int note = 69;

	void CalculateFrequencies();
	double GetOsc1();
	double GetOsc2();
	double GetOscillators();
	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
	}
};

