#include "Voice.h"

void Voice::SetOsc1Pitch(int coarse, double fine)
{
	osc1Factor = pitchFactor(coarse + fine);
}

void Voice::SetOsc2Pitch(int coarse, double fine)
{
	osc2Factor = pitchFactor(coarse + fine);
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

double Voice::GetOsc1Frequency(std::vector<double> &parameters, double fm, double lfoValue)
{
	double f = GetBaseFrequency() * osc1Factor;
	f *= lerp(1.0, parameters[volEnvPitch], volumeEnvelope.Get());
	f *= lerp(1.0, parameters[modEnvPitch], modEnvelope.Get());
	f *= 1 + parameters[lfoPitch] * GetLfoAmount(lfoValue);
	if (parameters[fmCoarse] < 0.0)
		f *= pitchFactor(fm * GetFmAmount(parameters, lfoValue));
	return f;
}

double Voice::GetOsc2Frequency(std::vector<double> &parameters, double fm, double lfoValue)
{
	double f = GetBaseFrequency() * osc2Factor;
	f *= lerp(1.0, parameters[volEnvPitch], volumeEnvelope.Get());
	f *= lerp(1.0, parameters[modEnvPitch], modEnvelope.Get());
	f *= 1 + parameters[lfoPitch] * GetLfoAmount(lfoValue);
	f *= lerp(1.0, parameters[volEnvOsc2], volumeEnvelope.Get());
	f *= lerp(1.0, parameters[modEnvOsc2], modEnvelope.Get());
	f *= 1 + parameters[lfoOsc2] * GetLfoAmount(lfoValue);
	if (parameters[fmCoarse] > 0.0)
		f *= pitchFactor(fm * GetFmAmount(parameters, lfoValue));
	return f;
}

double Voice::GetOscFm(double dt, std::vector<double>& parameters, double lfoValue)
{
	return oscFm.Next(dt, GetOsc1Frequency(parameters, 0.0, lfoValue), OscillatorWaveformSine);
}

double Voice::GetOsc1(double dt, std::vector<double> &parameters, double fm, double lfoValue)
{
	if (parameters[osc1Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc1Frequency(parameters, fm, lfoValue);
		out += osc1a.Next(dt, frequency / (1 + parameters[osc1Split]), (OscillatorWaveform)(int)parameters[osc1Wave]);
		out += osc1b.Next(dt, frequency * (1 + parameters[osc1Split]), (OscillatorWaveform)(int)parameters[osc1Wave]);
		return out;
	}
	return osc1a.Next(dt, GetOsc1Frequency(parameters, fm, lfoValue), (OscillatorWaveform)(int)parameters[osc1Wave]);
}

double Voice::GetOsc2(double dt, std::vector<double> &parameters, double fm, double lfoValue)
{
	if (parameters[osc2Split] > 0.0)
	{
		double out = 0.0;
		auto frequency = GetOsc2Frequency(parameters, fm, lfoValue);
		out += osc2a.Next(dt, frequency / (1 + parameters[osc2Split]), (OscillatorWaveform)(int)parameters[osc2Wave]);
		out += osc2b.Next(dt, frequency * (1 + parameters[osc2Split]), (OscillatorWaveform)(int)parameters[osc2Wave]);
		return out;
	}
	return osc2a.Next(dt, GetOsc2Frequency(parameters, fm, lfoValue), (OscillatorWaveform)(int)parameters[osc2Wave]);
}

double Voice::GetOscillators(double dt, std::vector<double> &parameters, double lfoValue)
{
	auto fm = GetOscFm(dt, parameters, lfoValue);
	double out = 0.0;
	if (parameters[oscMix] < 1.0)
		out += GetOsc1(dt, parameters, fm, lfoValue) * (1 - parameters[oscMix]);
	if (parameters[oscMix] > 0.0)
		out += GetOsc2(dt, parameters, fm, lfoValue) * parameters[oscMix];
	return out;
}

double Voice::GetFilterCutoff(std::vector<double>& parameters, double lfoValue)
{
	double cutoff = parameters[filterCutoff];
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
	filter.UpdateF(dt, GetFilterCutoff(parameters, lfoValue));

	UpdateEnvelopes(dt, parameters);
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators(dt, parameters, lfoValue);
	if (parameters[filterCutoff] < 20000.)
		for (int i = 0; i < 2; i++)
			out = filter.Process(out, dt, parameters[filterRes1], parameters[filterRes2]);
	out *= GetVolume();
	return out;
}