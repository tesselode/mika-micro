#pragma once

enum EnvelopeStage
{
	EnvelopeStageAttack,
	EnvelopeStageDecay,
	EnvelopeStageRelease
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
	void SetVelocityAmount(double a) { velocityAmount = a; }

	bool IsReleased() { return stage == EnvelopeStageRelease; }

	void SetVelocity(double v) { velocity = v / 128.0; }
	void Start() { stage = EnvelopeStageAttack; }
	void Release() { stage = EnvelopeStageRelease; }
	void Reset() { value = 0.0; }
	double Get() { return value * (1 - velocityAmount + velocityAmount * velocity); }
	void Update();

private:
	double dt = 44100;

	double attack = 100.0;
	double decay = 10.0;
	double sustain = 0.5;
	double release = 10.0;
	double velocityAmount = 0.0;
	double velocity = 0.0;

	EnvelopeStage stage = EnvelopeStageRelease;
	double value = 0.0;
};

