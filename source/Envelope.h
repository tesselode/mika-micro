#pragma once

enum class EnvelopeStages
{
	Attack,
	Decay,
	Release,
	Idle,
};

struct Envelope
{
	// these defaults are used for the lfo delay envelope
	double a = 0.0;
	double d = 0.5;
	double s = 1.0;
	double r = 0.5;

	EnvelopeStages stage = EnvelopeStages::Idle;
	double value = 0.0;

	bool IsReleased() { return stage == EnvelopeStages::Release || stage == EnvelopeStages::Idle; }

	void Reset() { value = 0.0; }
	void Start() { stage = EnvelopeStages::Attack; }
	void Release() { stage = EnvelopeStages::Release; }

	void Update(double dt)
	{
		switch (stage)
		{
		case EnvelopeStages::Attack:
			value += (1.1 - value) * a * dt;
			if (value >= 1.0)
			{
				value = 1.0;
				stage = EnvelopeStages::Decay;
			}
			break;
		case EnvelopeStages::Decay:
			value += (s - value) * d * dt;
			break;
		case EnvelopeStages::Release:
			value += (-.1 - value) * r * dt;
			if (value <= 0.0)
			{
				value = 0.0;
				stage = EnvelopeStages::Idle;
			}
			break;
		}
	}
};