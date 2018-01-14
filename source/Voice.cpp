#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	targetFrequency = PitchToFrequency(note);
}

double Voice::Next(double lfoValue)
{
	double out = 0.0;

	modEnvelope.Update();
	gateEnvelope.Update();
	lfoDelayEnvelope.Update();
	if (GetVolume() == 0) return 0.0;

	driftPhase += randMToN(-1.0, 1.0);
	driftPhase += (0 - driftPhase) * (1 / sampleRate);
	double driftValue = .0025 * sin(driftPhase * .001);

	frequency += (targetFrequency - frequency) * (glideSpeed / sampleRate);

	double fmAmount = fmCoarse + fmFine + fmEnvelopeAmount * modEnvelope.Get();

	// oscillator 1
	if (oscillatorMix < 1.0 || fmAmount > 0)
	{
		double oscOut = 0.0;

		double oscFrequency = frequency * oscillator1Coarse;
		oscFrequency *= 1 + driftValue;
		if (lfoAmount > 0.0)
		{
			oscFrequency *= 1 + lfoAmount * lfoValue * lfoDelayEnvelope.Get();
		}

		if (oscillator1Split > 1.0)
		{
			osc1a.SetFrequency(oscFrequency / oscillator1Split);
			osc1b.SetFrequency(oscFrequency * oscillator1Split);
			osc1b.Update();
			oscOut += osc1b.Get(oscillator1Wave);
		}
		else
		{
			osc1a.SetFrequency(oscFrequency);
		}
		osc1a.Update();
		oscOut += osc1a.Get(oscillator1Wave);

		out += oscOut * (1 - oscillatorMix);
	}

	// oscillator 2
	if (oscillatorMix > 0.0)
	{
		double oscOut = 0.0;

		double oscFrequency = frequency * oscillator2Coarse;
		oscFrequency *= 1 + driftValue;
		if (fmAmount > 0.0) oscFrequency *= PitchFactor(fmAmount * osc1a.Get(Sine));
		oscFrequency *= 1 + lfoAmount * lfoValue * lfoDelayEnvelope.Get();

		if (oscillator2Split > 1.0)
		{
			osc2a.SetFrequency(oscFrequency / oscillator2Split);
			osc2b.SetFrequency(oscFrequency * oscillator2Split);
			osc2b.Update();
			oscOut += osc2b.Get(oscillator2Wave);
		}
		else
		{
			osc2a.SetFrequency(oscFrequency);
		}
		osc2a.Update();
		oscOut += osc2a.Get(oscillator2Wave);

		out += oscOut * oscillatorMix;
	}
	
	double cutoff = filterCutoff;
	cutoff *= 1 + driftValue;
	cutoff += filterKeyTracking * (frequency - 440.0);
	cutoff += filterEnvelopeAmount * modEnvelope.Get();
	out = filter.Process(out, cutoff);

	out *= GetVolume();

	return out;
}
