#include "Envelope.h"

void Envelope::Update()
{
	switch (stage)
	{
	case kEnvelopeStageAttack:
		value = lerp(value, 1.1, attack * dt);
		if (value >= 1.0)
		{
			value = 1.0;
			stage = kEnvelopeStageDecay;
		}
		break;
	case kEnvelopeStageDecay:
		if (value > sustain)
		{
			value = lerp(value, sustain - .1, decay * dt);
			value = value < sustain ? sustain : value;
		}
		break;
	case kEnvelopeStageRelease:
		if (value > 0.0)
		{
			value = lerp(value, -.1, release * dt);
			value = value < 0 ? 0 : value;
		}
		break;
	}
}
