#include "Envelope.h"

void Envelope::Update(double dt, double a, double d, double s, double r)
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
