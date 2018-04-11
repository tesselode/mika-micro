#pragma once

enum EEnvelopeStage
{
	kAttack,
	kDecay,
	kRelease,
	kIdle
};

class Envelope
{
public:
	void Start() { stage = kAttack; }
	void Release() { stage = kRelease; }
	void Reset() { value = 0.0; }
	void Update(double dt, double a, double d, double s, double r);
	double Get(double velocitySensitivity, double velocity)
	{
		return value * (1.0 + (velocity - 1.0) * velocitySensitivity);
	}
	bool IsReleased() { return stage == kRelease || stage == kIdle; }

private:
	EEnvelopeStage stage = kIdle;
	double value = 0.0;
};

