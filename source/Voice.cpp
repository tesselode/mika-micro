#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
}

double Voice::Next()
{
	double out = 0.0;

	modEnvelope.Update();
	if (modEnvelope.Get() == 0.0)
	{
		return 0.0;
	}

	double frequency = PitchToFrequency(note);

	// oscillator 1
	if (oscillatorMix < 1.0)
	{
		double oscOut = 0.0;

		if (oscillator1Split > 1.0)
		{
			osc1a.SetFrequency(frequency / oscillator1Split);
			osc1b.SetFrequency(frequency * oscillator1Split);
			osc1b.Update();
			oscOut += osc1b.Get(Saw);
		}
		else
		{
			osc1a.SetFrequency(frequency);
		}
		osc1a.Update();
		oscOut += osc1a.Get(Saw);

		out += oscOut * (1 - oscillatorMix);
	}

	// oscillator 1
	if (oscillatorMix > 0.0)
	{
		double oscOut = 0.0;

		if (oscillator2Split > 1.0)
		{
			osc2a.SetFrequency(frequency / oscillator2Split);
			osc2b.SetFrequency(frequency * oscillator2Split);
			osc2b.Update();
			oscOut += osc2b.Get(Saw);
		}
		else
		{
			osc2a.SetFrequency(frequency);
		}
		osc2a.Update();
		oscOut += osc2a.Get(Saw);

		out += oscOut * oscillatorMix;
	}
	
	out *= modEnvelope.Get();

	return out;
}