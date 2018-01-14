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

	void SetSampleRate(double sr) { sampleRate = sr; }

	void SetAttack(double a) { attack = a; }
	void SetDecay(double d) { decay = d; }
	void SetSustain(double s) { sustain = s; }
	void SetRelease(double r) { release = r; }

	bool IsReleased() { return stage == EnvelopeStageRelease; }

	void Start() { stage = EnvelopeStageAttack; }
	void Release() { stage = EnvelopeStageRelease; }
	void Reset()
	{
		value = 0.0;
		Start();
	}
	double Get() { return value; }
	void Update();

private:
	double sampleRate = 44100;

	double attack = 100.0;
	double decay = 10.0;
	double sustain = 0.5;
	double release = 10.0;

	EnvelopeStage stage = EnvelopeStageRelease;
	double value = 0.0;
};

