#include "Voice.h"

void Voice::Start()
{
	baseFrequency = targetFrequency;
	if (GetVolume() == 0.0)
	{
		osc1a.Reset();
		osc1b.Reset(osc1SplitFactorA < 1.0 ? 0.33 : 0.0);
		osc2a.Reset();
		osc2b.Reset(osc2SplitFactorA < 1.0 ? 0.33 : 0.0);
		oscFm.Reset();
		volumeEnvelope.Reset();
		modEnvelope.Reset();
		delayEnvelope.Reset();
		filter.ResetF();
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

double Voice::GetFilterF(double lfoValue, double driftValue)
{
	double f = filterF;
	if (filterKeyTrack != 0.0) f += filterKeyTrack * baseFrequency * pitchBendFactor * .00005;
	if (volEnvCutoff != 0.0) f += volEnvCutoff * volumeEnvelope.Get();
	if (modEnvCutoff != 0.0) f += modEnvCutoff * modEnvelope.Get();
	if (lfoCutoff != 0.0) f += lfoCutoff * lfoValue;
	f *= 1 + driftValue * .00005;
	return f;
}

double Voice::GetOscillators(double lfoValue, double driftValue)
{
	double out = 0.0;

	double osc1BaseFrequency = baseFrequency * osc1TuneFactor * pitchBendFactor;
	if (lfoAmount < 0.0) osc1BaseFrequency *= 1 + fabs(lfoAmount) * lfoValue;
	osc1BaseFrequency *= 1 + driftValue;

	// fm
	double fmFactor = 1.0;
	if (fmCoarse != 0)
	{
		double fmAmount = fabs(fmCoarse) + fmFine;
		if (volEnvFm != 0.0) fmAmount += volEnvFm * volumeEnvelope.Get();
		if (modEnvFm != 0.0) fmAmount += modEnvFm * modEnvelope.Get();
		if (lfoFm != 0.0) fmAmount += lfoFm * lfoValue;
		double fmValue = oscFm.Next(osc1BaseFrequency, OscillatorWaveformSine);
		fmFactor = pitchFactor(fmAmount * fmValue);
	}

	// osc 1
	if (oscMix < 1.0)
	{
		if (fmCoarse < 0) osc1BaseFrequency *= fmFactor;
		auto osc1Out = 0.0;
		if (osc1SplitFactorA != 1.0)
		{
			osc1Out += osc1a.Next(osc1BaseFrequency * osc1SplitFactorA, osc1Wave);
			osc1Out += osc1b.Next(osc1BaseFrequency * osc1SplitFactorB, osc1Wave);
		}
		else
			osc1Out = osc1a.Next(osc1BaseFrequency, osc1Wave);
		out += osc1Out * (1.0 - oscMix);
	}

	// osc 2
	if (oscMix > 0.0)
	{
		double osc2BaseFrequency = baseFrequency * osc2TuneFactor * pitchBendFactor;
		osc2BaseFrequency *= 1 + driftValue;
		if (lfoAmount != 0.0) osc2BaseFrequency *= 1 + fabs(lfoAmount) * lfoValue;
		if (fmCoarse > 0) osc2BaseFrequency *= fmFactor;
		auto osc2Out = 0.0;
		if (osc2SplitFactorA != 1.0)
		{
			osc2Out += osc2a.Next(osc2BaseFrequency * osc2SplitFactorA, osc2Wave);
			osc2Out += osc2b.Next(osc2BaseFrequency * osc2SplitFactorB, osc2Wave);
		}
		else
			osc2Out = osc2a.Next(osc2BaseFrequency, osc2Wave);
		out += osc2Out * oscMix;
	}
	
	return out;
}

double Voice::GetDriftValue()
{
	double random = -1.0 + 2.0 * xorshf96() / 4294967296.0;
	double driftAccel = random;
	driftVelocity += driftAccel * 10000 * dt;
	driftVelocity -= driftVelocity * 2 * dt;
	driftPhase += driftVelocity * dt;
	return .0001 * sin(driftPhase);
}

double Voice::Next(double lfoValue)
{
	UpdateEnvelopes();
	if (GetVolume() == 0.0) return 0.0;

	if (baseFrequency != targetFrequency)
		baseFrequency = lerp(baseFrequency, targetFrequency, glideSpeed * dt);
	lfoValue *= delayEnvelope.Get();
	auto driftValue = GetDriftValue();

	auto out = GetOscillators(lfoValue, driftValue);
	if (filterF < 1.0)
	{
		auto f = GetFilterF(lfoValue, driftValue);
		for (int i = 0; i < 2; i++) out = filter.Process(out, f);
	}
	out *= GetVolume();
	return out;
}
