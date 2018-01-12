#include "Envelope.h"

void Envelope::Update()
{
	if (stage == EnvelopeStageAttack)
	{
		value += (1.5 - value) * (attack / sampleRate);
		if (value >= 1.0)
		{
			value = 1.0;
			stage = EnvelopeStageDecay;
		}
	}
	else if (stage == EnvelopeStageDecay)
	{
		value += (sustain - .5 - value) * (decay / sampleRate);
		if (value < sustain)
		{
			value = sustain;
		}
	}
	else if (stage == EnvelopeStageRelease)
	{
		value += (-.5 - value) * (release / sampleRate);
		if (value < 0)
		{
			value = 0;
		}
	}
}
