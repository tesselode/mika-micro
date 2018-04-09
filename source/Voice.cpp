#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	baseFrequency = pitchToFrequency(note);
}

void Voice::Start()
{
	volEnv.Start();
	modEnv.Start();
	lfoEnv.Start();
}

void Voice::Release()
{
	volEnv.Release();
	modEnv.Release();
	lfoEnv.Release();
}

double Voice::Next(double dt)
{
	volEnv.Update(dt, 1.0, 1.0, 0.5, 1.0);
	return osc1a.Next(dt, 440.0, kSaw) * GetVolume();
}
