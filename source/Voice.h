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
		volumeEnvelope.SetSampleRate(sr);
	}

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
	Oscillator osc1a;

	Envelope volumeEnvelope;

	int note = 69;
	double baseFrequency = 440.0;

	void UpdateEnvelopes()
	{
		volumeEnvelope.Update();
	}

	double GetOscillators();
};

