#include "Envelope.h"

void Envelope::Update()
{
	if (stage == EnvelopeStageAttack)
	{
		value += (1.5 - value) * (attack * dt);
		if (value >= 1.0)
		{
			value = 1.0;
			stage = EnvelopeStageDecay;
		}
	}
	else if (stage == EnvelopeStageDecay)
	{
		value += (sustain - .1 - value) * (decay * dt);
		value = value < sustain ? sustain : value;
	}
	else if (stage == EnvelopeStageRelease)
	{
		value += (-.1 - value) * (release * dt);
		value = value < 0 ? 0 : value;
	}
}
