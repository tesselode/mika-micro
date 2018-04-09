#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	baseFrequency = pitchToFrequency(note);
}

void Voice::Start()
{
	osc1a.Reset();
	osc1b.Reset(p[kOsc1Split] < 0.0 ? .33 : 0.0);
	osc2a.Reset();
	osc2b.Reset(p[kOsc2Split] < 0.0 ? .33 : 0.0);
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
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	if (GetVolume() == 0.0) return 0.0;

	auto out = 0.0;

	if (p[kOscMix] < 1.0)
	{
		out += osc1a.Next(dt, baseFrequency * (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
		if (p[kOsc1Split] != 0.0)
			out += osc1b.Next(dt, baseFrequency / (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
	}

	return out;
}
