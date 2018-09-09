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
	void Reset() { value = 0.0; }
	void Start() { stage = EnvelopeStages::Attack; }
	void Release() { stage = EnvelopeStages::Release; }
	void Update(double dt, double a, double d, double s, double r);
	double Get(double velocitySensitivity, double velocity)
	{
		return value * (1.0 + (velocity - 1.0) * velocitySensitivity);
	}
	bool IsReleased() { return stage == EnvelopeStages::Release || stage == EnvelopeStages::Idle; }

private:
	EnvelopeStages stage = EnvelopeStages::Idle;
	double value = 0.0;
};

