#include "Envelope.h"

void Envelope::Start()
{
	stage = EnvelopeStageAttack;
}

void Envelope::Release()
{
	stage = EnvelopeStageRelease;
}

void Envelope::Reset()
{
	value = 0.0;
}

void Envelope::Update(double dt, double a, double d, double s, double r)
{
	if (stage == EnvelopeStageAttack)
	{
		value = lerp(value, 1.1, a * dt);
		if (value > 1.0)
		{
			value = 1.0;
			stage = EnvelopeStageDecay;
		}
	}
	else if (stage == EnvelopeStageDecay)
	{
		value = lerp(value, s - .1, d * dt);
		if (value < s) value = s;
	}
	else if (stage == EnvelopeStageRelease)
	{
		value = lerp(value, -.1, r * dt);
		if (value < 0.0) value = 0.0;
	}
}

double Envelope::Get()
{
	return value;
}

bool Envelope::IsReleased()
{
	return stage == EnvelopeStageRelease;
}
