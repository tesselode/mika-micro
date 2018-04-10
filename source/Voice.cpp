#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	baseFrequency = pitchToFrequency(note);
}

void Voice::Start()
{
	oscFm.Reset();
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
	// skip processing if voice is silent
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	if (GetVolume() == 0.0) return 0.0;

	// oscillator base frequencies
	auto osc1Frequency = baseFrequency * osc1PitchFactor;
	auto osc2Frequency = baseFrequency * osc2PitchFactor;

	// fm
	switch ((int)p[kFmMode])
	{
	case 0:
		break;
	case 1:
	case 2:
	{
		auto fmAmount = p[kFmCoarse] + p[kFmFine];
		auto fmValue = pitchFactor(oscFm.Next(dt, osc1Frequency, kSine) * fmAmount);
		switch ((int)p[kFmMode])
		{
		case 1:
			osc1Frequency *= fmValue;
			break;
		case 2:
			osc2Frequency *= fmValue;
			break;
		}
		break;
	}
	}

	// oscillator 1
	auto osc1Out = 0.0;
	if (p[kOscMix] < 1.0)
	{
		osc1Out += osc1a.Next(dt, osc1Frequency * (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
		if (p[kOsc1Split] != 0.0)
			osc1Out += osc1b.Next(dt, osc1Frequency / (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[kOscMix] > 0.0)
	{
		osc2Out += osc2a.Next(dt, osc2Frequency * (1.0 + p[kOsc2Split]), (EWaveforms)(int)p[kOsc2Wave]);
		if (p[kOsc2Split] != 0.0)
			osc2Out += osc2b.Next(dt, osc2Frequency / (1.0 + p[kOsc2Split]), (EWaveforms)(int)p[kOsc2Wave]);
	}

	// oscillator mix
	auto out = osc1Out * (1.0 - p[kOscMix]) + osc2Out * p[kOscMix];
	out /= (1.0 + abs(.5 - p[kOscMix])) * 1.5;

	out *= GetVolume();

	// filter
	if (p[kFilterEnabled])
		out = filter.Process(dt, out, p[kFilterCutoff], p[kFilterResonance]);

	return out;
}
