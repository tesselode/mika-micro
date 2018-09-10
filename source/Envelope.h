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
	double a = 0.0;
	double d = 0.0;
	double s = 0.0;
	double r = 0.0;
	EnvelopeStages stage = EnvelopeStages::Idle;
	double value = 0.0;

	bool IsReleased() { return stage == EnvelopeStages::Release || stage == EnvelopeStages::Idle; }

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