#include "Envelope.h"

void Envelope::Update(double dt, double a, double d, double s, double r)
{
	switch (stage)
	{
	case kAttack:
		value += (1.1 - value) * a * dt;
		if (value >= 1.0)
		{
			value = 1.0;
			stage = kDecay;
		}
		break;
	case kDecay:
		value += (s - value) * d * dt;
		break;
	case kRelease:
		value += (-.1 - value) * r * dt;
		if (value <= 0.0)
		{
			value = 0.0;
			stage = kIdle;
		}
		break;
	}
}
