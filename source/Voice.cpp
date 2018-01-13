#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
}

double Voice::Next()
{
	modEnvelope.Update();

	double frequency = PitchToFrequency(note);
	osc1a.SetFrequency(frequency);
	osc1b.SetFrequency(frequency * 1.01);
	osc2a.SetFrequency(frequency / 1.01);
	osc2b.SetFrequency(frequency * 1.02);

	osc1a.Update();
	osc1b.Update();
	osc2a.Update();
	osc2b.Update();
	
	double out = 0.0;

	out += osc1a.Get(Saw);
	out += osc1b.Get(Saw);
	out += osc2a.Get(Saw);
	out += osc2b.Get(Saw);

	out *= modEnvelope.Get();

	return out;
}