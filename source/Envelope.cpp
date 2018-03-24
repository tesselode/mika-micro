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
		value = lerp(value, sustain - .1, decay * dt);
		value = value < sustain ? sustain : value;
		break;
	case kEnvelopeStageRelease:
		value = lerp(value, -.1, release * dt);
		value = value < 0 ? 0 : value;
		break;
	}
}
