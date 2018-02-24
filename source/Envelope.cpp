#include "Envelope.h"

void Envelope::Update()
{
	switch (stage)
	{
	case EnvelopeStageAttack:
		value = lerp(value, 1.1, attack * dt);
		if (value >= 1.0)
		{
			value = 1.0;
			stage = EnvelopeStageDecay;
		}
		break;
	case EnvelopeStageDecay:
		if (value > sustain)
		{
			value = lerp(value, sustain - .1, decay * dt);
			value = value < sustain ? sustain : value;
		}
		break;
	case EnvelopeStageRelease:
		if (value > 0.0)
		{
			value = lerp(value, -.1, release * dt);
			value = value < 0 ? 0 : value;
		}
		break;
	}
}
