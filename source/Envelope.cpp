#include "Envelope.h"

void Envelope::Update(double dt, double a, double d, double s, double r)
{
	switch (stage)
	{
	case EnvelopeStages::Attack:
		value += (1.0 - value) * a * dt;
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
		value -= value * r * dt;
		break;
	}
}
