#include "Voice.h"

Voice::Voice(std::vector<double>& params, int seed)
	:parameters(params),
	gen(seed),
	dist(-1.0, 1.0)
{
}

int Voice::GetNote()
{
	return note;
}

void Voice::SetNote(int n)
{
	note = n;
	baseFrequency = pitchToFrequency(note);
}

double Voice::GetVolume()
{
	return volumeEnvelope.Get();
}

bool Voice::IsReleased()
{
	return volumeEnvelope.IsReleased();
}

void Voice::SetPitchBendFactor(double v)
{
	pitchBendFactor = v;
}

void Voice::SetOsc1Pitch(int coarse, double fine)
{
	osc1Factor = pitchFactor(coarse + fine);
}

void Voice::SetOsc2Pitch(int coarse, double fine)
{
	osc2Factor = pitchFactor(coarse + fine);
}

void Voice::Start()
{
	if (GetVolume() == 0.0) Reset();
	volumeEnvelope.Start();
	modEnvelope.Start();
	delayEnvelope.Start();
}

void Voice::Release()
{
	volumeEnvelope.Release();
	modEnvelope.Release();
	delayEnvelope.Release();
}

double Voice::GetBaseFrequency()
{
	return baseFrequency * pitchBendFactor;
}

double Voice::GetLfoAmount()
{
	return lfoValue * delayEnvelope.Get();
}

double Voice::GetFmAmount()
{
	double fm = fabs(parameters[fmCoarse]) + parameters[fmFine];
	fm += parameters[volEnvFm] * volumeEnvelope.Get();
	fm += parameters[modEnvFm] * modEnvelope.Get();
	fm += parameters[lfoFm] * GetLfoAmount();
	return fm;
}

double Voice::GetOsc1Frequency(bool applyFm)
{
	double f = GetBaseFrequency() * osc1Factor;
	f *= 1 + driftValue;
	if (parameters[lfoAmount] < 0.0)
		f *= 1 + parameters[lfoAmount] * GetLfoAmount();
	if (applyFm && parameters[fmCoarse] < 0.0)
		f *= pitchFactor(fmValue * GetFmAmount());
	return f;
}

double Voice::GetOsc2Frequency(bool applyFm)
{
	double f = GetBaseFrequency() * osc2Factor;
	f *= 1 + driftValue;
	f *= 1 + parameters[lfoAmount] * GetLfoAmount();
	if (applyFm && parameters[fmCoarse] > 0.0)
		f *= pitchFactor(fmValue * GetFmAmount());
	return f;
}

double Voice::GetOscFm()
{
	return oscFm.Next(deltaTime, GetOsc1Frequency(false), OscillatorWaveformSine);
}

double Voice::GetOsc1()
{
	auto waveform = (OscillatorWaveform)(int)parameters[osc1Wave];
	auto frequency = GetOsc1Frequency(true);
	if (parameters[osc1Split] > 0.0)
	{
		double out = 0.0;
		out += osc1a.Next(deltaTime, frequency / (1 + parameters[osc1Split]), waveform);
		out += osc1b.Next(deltaTime, frequency * (1 + parameters[osc1Split]), waveform);
		return out;
	}
	return osc1a.Next(deltaTime, frequency, waveform);
}

double Voice::GetOsc2()
{
	auto waveform = (OscillatorWaveform)(int)parameters[osc2Wave];
	auto frequency = GetOsc2Frequency(true);
	if (parameters[osc2Split] > 0.0)
	{
		double out = 0.0;
		out += osc2a.Next(deltaTime, frequency / (1 + parameters[osc2Split]), waveform);
		out += osc2b.Next(deltaTime, frequency * (1 + parameters[osc2Split]), waveform);
		return out;
	}
	return osc2a.Next(deltaTime, frequency, waveform);
}

double Voice::GetOscillators()
{
	fmValue = GetOscFm();
	double out = 0.0;
	if (parameters[oscMix] < 1.0)
		out += GetOsc1() * (1 - parameters[oscMix]);
	if (parameters[oscMix] > 0.0)
		out += GetOsc2() * parameters[oscMix];
	return out;
}

double Voice::GetFilterCutoff()
{
	double cutoff = parameters[filterCutoff];
	cutoff *= 1 + driftValue;
	cutoff += GetBaseFrequency() * parameters[filterKeyTrack];
	cutoff += parameters[volEnvCutoff] * volumeEnvelope.Get();
	cutoff += parameters[modEnvCutoff] * modEnvelope.Get();
	cutoff += parameters[lfoCutoff] * GetLfoAmount();
	return cutoff;
}

void Voice::UpdateDrift()
{
	driftPhase += dist(gen);
	driftPhase -= driftPhase * deltaTime;
	driftValue = .01 * sin(driftPhase * 10 * deltaTime);
}

void Voice::UpdateEnvelopes()
{
	volumeEnvelope.Update(deltaTime, parameters[volEnvA], parameters[volEnvD], parameters[volEnvS], parameters[volEnvR]);
	modEnvelope.Update(deltaTime, parameters[modEnvA], parameters[modEnvD], parameters[modEnvS], parameters[modEnvR]);
	delayEnvelope.Update(deltaTime, parameters[lfoDelay], 1.0, 1.0, 0.1);
}

void Voice::Reset()
{
	osc1a.ResetPhase();
	osc1b.ResetPhase();
	osc2a.ResetPhase();
	osc2b.ResetPhase();
	volumeEnvelope.Reset();
	modEnvelope.Reset();
	delayEnvelope.Reset();
}

double Voice::Next(double dt, double lfo)
{
	deltaTime = dt;
	lfoValue = lfo;

	UpdateDrift();
	filter.UpdateF(dt, GetFilterCutoff());
	UpdateEnvelopes();

	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators();
	if (parameters[filterCutoff] < 20000.)
		for (int i = 0; i < 2; i++)
			out = filter.Process(out, dt, parameters[filterRes1], parameters[filterRes2]);
	out *= GetVolume();
	return out;
}