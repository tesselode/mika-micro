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

private:
	double dt = 0.0;

	double attack = 0.0;
	double decay = 0.0;
	double sustain = 0.0;
	double release = 0.0;

	EnvelopeStage stage = EnvelopeStageRelease;
	double value = 0.0;
};

