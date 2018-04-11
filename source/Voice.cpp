#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	targetFrequency = pitchToFrequency(note);
}

void Voice::Start()
{
	if (GetVolume() == 0.0)
	{
		oscFm.Reset();
		osc1a.Reset();
		osc1b.Reset(p[kOsc1Split] < 0.0 ? .33 : 0.0);
		osc1bMix = p[kOsc1Split] != 0.0 ? 1.0 : 0.0;
		osc2a.Reset();
		osc2b.Reset(p[kOsc2Split] < 0.0 ? .33 : 0.0);
		osc2bMix = p[kOsc2Split] != 0.0 ? 1.0 : 0.0;
		volEnv.Reset();
		modEnv.Reset();
		lfoEnv.Reset();
		filter.Reset();
	}
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

double Voice::Next(double dt, double lfoValue, double driftValue)
{
	// skip processing if voice is silent
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	auto volEnvValue = volEnv.Get(p[kVolEnvV], velocity);
	if (volEnvValue == 0.0 && filter.IsSilent()) return 0.0;

	// update envelopes
	modEnv.Update(dt, p[kModEnvA], p[kModEnvD], p[kModEnvS], p[kModEnvR]);
	auto modEnvValue = modEnv.Get(p[kModEnvV], velocity);
	lfoEnv.Update(dt, p[kLfoDelay], 0.5, 1.0, 0.5);
	lfoValue *= lfoEnv.Get(0.0, velocity);

	// mono glide
	baseFrequency += (targetFrequency - baseFrequency) * p[kGlideSpeed] * dt;

	// oscillator base frequencies
	auto osc1Frequency = baseFrequency * osc1PitchFactor * pitchBendFactor * (1.0 + driftValue);
	if (p[kLfoAmount] < 0.0) osc1Frequency *= 1.0 + abs(p[kLfoAmount]) * lfoValue;
	auto osc2Frequency = baseFrequency * osc2PitchFactor * pitchBendFactor * (1.0 + driftValue);
	osc2Frequency *= 1.0 + abs(p[kLfoAmount]) * lfoValue;

	// oscillator split smoothing
	osc1bMix += ((p[kOsc1Split] != 0.0 ? 1.0 : 0.0) - osc1bMix) * 100.0 * dt;
	osc2bMix += ((p[kOsc2Split] != 0.0 ? 1.0 : 0.0) - osc2bMix) * 100.0 * dt;

	// fm
	switch ((int)p[kFmMode])
	{
	case 1:
	case 2:
	{
		auto fmAmount = p[kFmCoarse] + p[kFmFine];
		fmAmount += p[kVolEnvFm] * volEnvValue;
		fmAmount += p[kModEnvFm] * modEnvValue;
		fmAmount += p[kLfoFm] * lfoValue;
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
	if (p[kOscMix] < .99)
	{
		osc1Out += osc1a.Next(dt, osc1Frequency * (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
		if (osc1bMix > .01)
			osc1Out += osc1bMix * osc1b.Next(dt, osc1Frequency / (1.0 + p[kOsc1Split]), (EWaveforms)(int)p[kOsc1Wave]);
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[kOscMix] > .01)
	{
		osc2Out += osc2a.Next(dt, osc2Frequency * (1.0 + p[kOsc2Split]), (EWaveforms)(int)p[kOsc2Wave]);
		if (osc2bMix > .01)
			osc2Out += osc2bMix * osc2b.Next(dt, osc2Frequency / (1.0 + p[kOsc2Split]), (EWaveforms)(int)p[kOsc2Wave]);
	}

	// oscillator mix
	auto out = osc1Out * (1.0 - p[kOscMix]) + osc2Out * p[kOscMix];
	out /= (1.0 + abs(.5 - p[kOscMix])) * 1.5;

	out *= GetVolume();

	// filter
	filterMix += ((p[kFilterEnabled] ? 1.0 : 0.0) - filterMix) * 100.0 * dt;
	if (filterMix > .01)
	{
		auto cutoff = p[kFilterCutoff];
		cutoff *= 1.0 + driftValue;
		cutoff += p[kVolEnvCutoff] * volEnvValue;
		cutoff += p[kModEnvCutoff] * modEnvValue;
		cutoff += lfoValue * p[kLfoCutoff];
		cutoff += p[kFilterKeyTrack] * baseFrequency * pitchBendFactor;
		auto filterOut = filter.Process(dt, out, cutoff, p[kFilterResonance]);
		out = out * (1.0 - filterMix) + filterOut * filterMix;
	}

	return out;
}
