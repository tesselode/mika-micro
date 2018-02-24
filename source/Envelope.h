#pragma once

#include "Util.h"

enum EnvelopeStage
{
	EnvelopeStageAttack,
	EnvelopeStageDecay,
	EnvelopeStageRelease,
};

class Envelope
{
public:
	Envelope() {};
	Envelope(double a, double d, double s, double r)
	{
		SetAttack(a);
		SetDecay(d);
		SetSustain(s);
		SetRelease(r);
	}

	void SetSampleRate(double sr) { dt = 1.0 / sr; }

	void SetAttack(double a) { attack = a; }
	void SetDecay(double d) { decay = d; }
	void SetSustain(double s) { sustain = s; }
	void SetRelease(double r) { release = r; }

	void Start() { stage = EnvelopeStageAttack; }
	void Release() { stage = EnvelopeStageRelease; }
	void Reset() { value = 0.0; }
	void Update();
	double Get() { return value; }
	bool IsReleased() { return stage == EnvelopeStageRelease; }

private:
	double dt = 0.0;

	double attack = 0.0;
	double decay = 0.0;
	double sustain = 0.0;
	double release = 0.0;

	EnvelopeStage stage = EnvelopeStageRelease;
	double value = 0.0;
};

