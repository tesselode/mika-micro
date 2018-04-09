#pragma once

enum EEnvelopeStage
{
	kAttack,
	kDecay,
	kSustain,
	kRelease,
	kIdle
};

class Envelope
{
public:
	void Start() { stage = kAttack; }
	void Release() { stage = kRelease; }
	void Update(double dt, double a, double d, double s, double r);
	double Get() { return value; }
	bool IsReleased() { return stage == kRelease || stage == kIdle; }

private:
	EEnvelopeStage stage = kIdle;
	double value = 0.0;
};

