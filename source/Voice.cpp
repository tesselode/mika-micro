#include "Voice.h"

void Voice::SetOsc1Pitch(int coarse, double fine)
{
	osc1Factor = pitchFactor(coarse + fine);
}

void Voice::SetOsc2Pitch(int coarse, double fine)
{
	osc2Factor = pitchFactor(coarse + fine);
}

Voice::Voice(int seed)
	:gen(seed),
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

void Voice::Start()
{
	if (GetVolume() == 0.0)
	{
		osc1a.ResetPhase();
		osc1b.ResetPhase();
		osc2a.ResetPhase();
		osc2b.ResetPhase();
		volumeEnvelope.Reset();
		modEnvelope.Reset();
		delayEnvelope.Reset();
	}
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

double Voice::GetLfoAmount(double lfoValue)
{
	return lfoValue * delayEnvelope.Get();
}

double Voice::GetFmAmount(std::vector<double>& parameters, double lfoValue)
{
	double fm = fabs(parameters[fmCoarse]) + parameters[fmFine];
	fm += parameters[volEnvFm] * volumeEnvelope.Get();
	fm += parameters[modEnvFm] * modEnvelope.Get();
	fm += parameters[lfoFm] * GetLfoAmount(lfoValue);
	return fm;
}

double Voice::GetOsc1Frequency(std::vector<double> &parameters, double fm, double lfoValue, double driftValue)
{
	double f = GetBaseFrequency() * osc1Factor;
	f *= 1 + driftValue;
	if (parameters[lfoAmount] < 0.0)
		f *= 1 + parameters[lfoAmount] * GetLfoAmount(lfoValue);
	if (parameters[fmCoarse] < 0.0)
		f *= pitchFactor(fm * GetFmAmount(parameters, lfoValue));
	return f;
}

double Voice::GetOsc2Frequency(std::vector<double> &parameters, double fm, double lfoValue, double driftValue)
{
	double f = GetBaseFrequency() * osc2Factor;
	f *= 1 + driftValue;
	f *= 1 + parameters[lfoAmount] * GetLfoAmount(lfoValue);
	if (parameters[fmCoarse] > 0.0)
		f *= pitchFactor(fm * GetFmAmount(parameters, lfoValue));
	return f;
}

double Voice::GetOscFm(double dt, std::vector<double>& parameters, double lfoValue, double driftValue)
{
	return oscFm.Next(dt, GetOsc1Frequency(parameters, 0.0, lfoValue, driftValue), OscillatorWaveformSine);
}

double Voice::GetOsc1(double dt, std::vector<double> &parameters, double fm, double lfoValue, double driftValue)
{
	if (parameters[osc1Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc1Frequency(parameters, fm, lfoValue, driftValue);
		out += osc1a.Next(dt, frequency / (1 + parameters[osc1Split]), (OscillatorWaveform)(int)parameters[osc1Wave]);
		out += osc1b.Next(dt, frequency * (1 + parameters[osc1Split]), (OscillatorWaveform)(int)parameters[osc1Wave]);
		return out;
	}
	return osc1a.Next(dt, GetOsc1Frequency(parameters, fm, lfoValue, driftValue), (OscillatorWaveform)(int)parameters[osc1Wave]);
}

double Voice::GetOsc2(double dt, std::vector<double> &parameters, double fm, double lfoValue, double driftValue)
{
	if (parameters[osc2Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc2Frequency(parameters, fm, lfoValue, driftValue);
		out += osc2a.Next(dt, frequency / (1 + parameters[osc2Split]), (OscillatorWaveform)(int)parameters[osc2Wave]);
		out += osc2b.Next(dt, frequency * (1 + parameters[osc2Split]), (OscillatorWaveform)(int)parameters[osc2Wave]);
		return out;
	}
	return osc2a.Next(dt, GetOsc2Frequency(parameters, fm, lfoValue, driftValue), (OscillatorWaveform)(int)parameters[osc2Wave]);
}

double Voice::GetOscillators(double dt, std::vector<double> &parameters, double lfoValue, double driftValue)
{
	auto fm = GetOscFm(dt, parameters, lfoValue, driftValue);
	double out = 0.0;
	if (parameters[oscMix] < 1.0)
		out += GetOsc1(dt, parameters, fm, lfoValue, driftValue) * (1 - parameters[oscMix]);
	if (parameters[oscMix] > 0.0)
		out += GetOsc2(dt, parameters, fm, lfoValue, driftValue) * parameters[oscMix];
	return out;
}

double Voice::GetFilterCutoff(std::vector<double>& parameters, double lfoValue, double driftValue)
{
	double cutoff = parameters[filterCutoff];
	cutoff *= 1 + driftValue;
	cutoff += GetBaseFrequency() * parameters[filterKeyTrack];
	cutoff += parameters[volEnvCutoff] * volumeEnvelope.Get();
	cutoff += parameters[modEnvCutoff] * modEnvelope.Get();
	cutoff += parameters[lfoCutoff] * GetLfoAmount(lfoValue);
	return cutoff;
}

void Voice::UpdateEnvelopes(double dt, std::vector<double>& parameters)
{
	volumeEnvelope.Update(dt, parameters[volEnvA], parameters[volEnvD], parameters[volEnvS], parameters[volEnvR]);
	modEnvelope.Update(dt, parameters[modEnvA], parameters[modEnvD], parameters[modEnvS], parameters[modEnvR]);
	delayEnvelope.Update(dt, parameters[lfoDelay], 1.0, 1.0, 0.1);
}

double Voice::Next(double dt, std::vector<double> &parameters, double lfoValue)
{
	driftPhase += dist(gen);
	driftPhase -= driftPhase * dt;
	double driftValue = .01 * sin(driftPhase * 10 * dt);

	filter.UpdateF(dt, GetFilterCutoff(parameters, lfoValue, driftValue));

	UpdateEnvelopes(dt, parameters);
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators(dt, parameters, lfoValue, driftValue);
	if (parameters[filterCutoff] < 20000.)
		for (int i = 0; i < 2; i++)
			out = filter.Process(out, dt, parameters[filterRes1], parameters[filterRes2]);
	out *= GetVolume();
	return out;
}