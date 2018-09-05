#pragma once

enum class EnvelopeStages
{
	Attack,
	Decay,
	Release,
	Idle
};

class Envelope
{
public:
	void Start() { stage = EnvelopeStages::Attack; }
	void Release() { stage = EnvelopeStages::Release; }
	void Update(double dt, double a, double d, double s, double r);
	double Get() { return value; }
	bool IsReleased() { return stage == EnvelopeStages::Release || stage == EnvelopeStages::Idle; }

private:
	EnvelopeStages stage = EnvelopeStages::Idle;
	double value = 0.0;
};

