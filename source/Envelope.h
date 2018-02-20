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
	void Start();
	void Release();
	void Reset();
	void Update(double dt, double a, double d, double s, double r);
	double Get(double velocitySensitivity, int velocity);
	bool IsReleased();

private:
	EnvelopeStage stage = EnvelopeStageRelease;
	double value = 0.0;
};

