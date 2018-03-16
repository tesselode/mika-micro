#pragma once

#include "Util.h"

enum EEnvelopeStages
{
	kEnvelopeStageAttack,
	kEnvelopeStageDecay,
	kEnvelopeStageRelease,
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
	void SetVelocitySensitivity(double v) { velocitySensitivity = v; }

	void SetVelocity(double v) { velocity = v; }
	void Start() { stage = kEnvelopeStageAttack; }
	void Release() { stage = kEnvelopeStageRelease; }
	void Reset() { value = 0.0; }
	void Update();
	double Get() { return value * (1 - velocitySensitivity + velocity * velocitySensitivity); }
	bool IsReleased() { return stage == kEnvelopeStageRelease; }

private:
	double dt = 0.0;

	double attack = 0.0;
	double decay = 0.0;
	double sustain = 0.0;
	double release = 0.0;
	double velocitySensitivity = 0.0;

	EEnvelopeStages stage = kEnvelopeStageRelease;
	double value = 0.0;
	double velocity = 0.0;
};

