#include "Voice.h"

void Voice::SetNote(int n)
{
	note = n;
	targetFrequency = pitchToFrequency(note);
}

void Voice::Reset()
{
	oscFm.Reset();
	osc1a.Reset();
	osc1b.Reset(p[Osc1Split] < 0.0 ? .33 : 0.0);
	osc2a.Reset();
	osc2b.Reset(p[Osc2Split] < 0.0 ? .33 : 0.0);
	volEnv.Reset();
	modEnv.Reset();
	lfoEnv.Reset();

	// set smoothed variables directly to their target values
	osc1bMix = p[Osc1Split] != 0.0 ? 1.0 : 0.0;
	osc2bMix = p[Osc2Split] != 0.0 ? 1.0 : 0.0;
}

void Voice::Start()
{
	if (GetVolume() == 0.0) Reset();
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

void Voice::UpdateEnvelopes(double dt)
{
	volEnv.Update(dt, p[VolEnvA], p[VolEnvD], p[VolEnvS], p[VolEnvR]);
	modEnv.Update(dt, p[ModEnvA], p[ModEnvD], p[ModEnvS], p[ModEnvR]);
	lfoEnv.Update(dt, p[LfoDelay], 0.5, 1.0, 0.5);
}

double Voice::GetFilterCutoff(double volEnvValue, double modEnvValue, double lfoValue, double driftValue)
{
	auto cutoff = p[FilterCutoff];
	cutoff *= 1.0 + driftValue;
	cutoff += p[VolEnvCutoff] * volEnvValue;
	cutoff += p[ModEnvCutoff] * modEnvValue;
	cutoff += lfoValue * p[LfoCutoff];
	cutoff += p[FilterKeyTrack] * baseFrequency * pitchBendFactor;
	return cutoff;
}

double Voice::Next(double dt, double lfoValue, double driftValue)
{
	// update envelopes
	UpdateEnvelopes(dt);
	auto volEnvValue = volEnv.Get(p[VolEnvV], velocity);
	auto modEnvValue = modEnv.Get(p[ModEnvV], velocity);

	// skip processing if voice is silent
	if (volEnvValue == 0.0 && filter.IsSilent()) return 0.0;

	// apply lfo delay
	lfoValue *= lfoEnv.Get(0.0, velocity);

	// glide to target frequency (for mono and legato modes)
	baseFrequency += (targetFrequency - baseFrequency) * p[GlideSpeed] * dt;

	// smooth oscillator split
	auto osc1bEnabled = p[Osc1Split] != 0.0 && osc1b.GetWaveform() != Waveforms::Noise;
	auto osc2bEnabled = p[Osc2Split] != 0.0 && osc2b.GetWaveform() != Waveforms::Noise;
	osc1bMix += ((osc1bEnabled ? 1.0 : 0.0) - osc1bMix) * 100.0 * dt;
	osc2bMix += ((osc2bEnabled ? 1.0 : 0.0) - osc2bMix) * 100.0 * dt;

	// calculate oscillator base frequencies
	auto osc1Frequency = baseFrequency * osc1PitchFactor * pitchBendFactor * (1.0 + driftValue);
	if (p[LfoAmount] < 0.0) osc1Frequency *= 1.0 + abs(p[LfoAmount]) * lfoValue;
	auto osc2Frequency = baseFrequency * osc2PitchFactor * pitchBendFactor * (1.0 + driftValue);
	osc2Frequency *= 1.0 + abs(p[LfoAmount]) * lfoValue;

	// fm
	switch ((int)p[FmMode])
	{
	case 1:
	case 2:
	{
		auto fmAmount = p[FmCoarse] + p[FmFine];
		fmAmount += p[VolEnvFm] * volEnvValue;
		fmAmount += p[ModEnvFm] * modEnvValue;
		fmAmount += p[LfoFm] * lfoValue;
		auto fmValue = pitchFactor(oscFm.Next(dt, osc1Frequency) * fmAmount);
		switch ((int)p[FmMode])
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

	auto out = 0.0;

	// oscillator 1
	auto osc1Out = 0.0;
	if (p[OscMix] < .99)
	{
		osc1Out += osc1a.Next(dt, osc1Frequency * osc1SplitFactorA);
		if (osc1bMix > .01)
			osc1Out += osc1bMix * osc1b.Next(dt, osc1Frequency * osc1SplitFactorB);
		out += osc1Out * sqrt(1.0 - p[OscMix]);
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[OscMix] > .01)
	{
		osc2Out += osc2a.Next(dt, osc2Frequency * osc2SplitFactorA);
		if (osc2bMix > .01)
			osc2Out += osc2bMix * osc2b.Next(dt, osc2Frequency * osc2SplitFactorB);
		out += osc2Out * sqrt(p[OscMix]);
	}

	// apply volume envelope
	out *= GetVolume();

	// filter
	auto cutoff = GetFilterCutoff(volEnvValue, modEnvValue, lfoValue, driftValue);
	out = filter.Process(dt, out, cutoff, p[FilterResonance]);

	return out;
}
