#include "Voice.h"

void Voice::Start()
{
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

double Voice::GetFilterCutoff(double lfoValue, double driftValue)
{
	double c = filterCutoff;
	if (filterKeyTrack != 0.0) c += filterKeyTrack * baseFrequency * pitchBendFactor;
	if (volEnvCutoff != 0.0) c += volEnvCutoff * volumeEnvelope.Get();
	if (modEnvCutoff != 0.0) c += modEnvCutoff * modEnvelope.Get();
	if (lfoCutoff != 0.0)
	{
		auto lc = abs(lfoCutoff);
		lc *= .000125;
		lc *= lc;
		lc *= 8000.0;
		if (lfoCutoff < 0) lc *= -1;
		c += lc * lfoValue;
	}
	c *= 1 + driftValue;
	return c;
}

double Voice::GetOscillators(double lfoValue, double driftValue)
{
	double out = 0.0;

	double osc1BaseFrequency = baseFrequency * osc1TuneFactor * pitchBendFactor;
	if (lfoAmount < 0.0) osc1BaseFrequency *= 1 + fabs(lfoAmount) * lfoValue;
	osc1BaseFrequency *= 1 + driftValue;

	// fm
	double fmFactor = 1.0;
	if (fmMode != 0)
	{
		double fmAmount = fmCoarse + fmFine;
		if (volEnvFm != 0.0) fmAmount += volEnvFm * volumeEnvelope.Get();
		if (modEnvFm != 0.0) fmAmount += modEnvFm * modEnvelope.Get();
		if (lfoFm != 0.0) fmAmount += lfoFm * lfoValue;
		double fmValue = oscFm.Next(osc1BaseFrequency, kWaveformSine);
		fmFactor = pitchFactor(fmAmount * fmValue);
	}

	// osc 1
	if (oscMix < 1.0)
	{
		if (fmMode == 1 && osc1Wave != kWaveformNoise) osc1BaseFrequency *= fmFactor;
		auto osc1Out = 0.0;
		if (osc1SplitEnabled && osc1Wave != kWaveformNoise)
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
		if (fmMode == 2 && osc2Wave != kWaveformNoise) osc2BaseFrequency *= fmFactor;
		auto osc2Out = 0.0;
		if (osc2SplitEnabled && osc2Wave != kWaveformNoise)
		{
			osc2Out += osc2a.Next(osc2BaseFrequency * osc2SplitFactorA, osc2Wave);
			osc2Out += osc2b.Next(osc2BaseFrequency * osc2SplitFactorB, osc2Wave);
		}
		else
			osc2Out = osc2a.Next(osc2BaseFrequency, osc2Wave);
		out += osc2Out * oscMix;
	}
	
	return out / (1 + abs(.5 - oscMix)) * 1.5;
}

void Voice::UpdateSplitAndWave()
{
	// applies fade out and fade in for parameters that would make clicking sounds on change
	double fadeSpeed = 100 * dt;
	if (osc1WaveNext != kWaveformNone ||
		osc2WaveNext != kWaveformNone ||
		osc1SplitEnabled != osc1SplitEnabledNext ||
		osc2SplitEnabled != osc2SplitEnabledNext ||
		filterEnabled != filterEnabledNext)
	{
		fadeVolume -= fadeSpeed;
		if (fadeVolume <= 0)
		{
			fadeVolume = 0;
			if (osc1WaveNext != kWaveformNone)
			{
				osc1Wave = osc1WaveNext;
				osc1WaveNext = kWaveformNone;
			}
			if (osc2WaveNext != kWaveformNone)
			{
				osc2Wave = osc2WaveNext;
				osc2WaveNext = kWaveformNone;
			}
			osc1SplitEnabled = osc1SplitEnabledNext;
			osc2SplitEnabled = osc2SplitEnabledNext;
			filterEnabled = filterEnabledNext;
		}
	}
	else if (fadeVolume < 1.0)
	{
		fadeVolume += fadeSpeed;
		if (fadeVolume > 1) fadeVolume = 1;
	}
}

double Voice::Next(double lfoValue, double driftValue)
{
	// parameter smoothing
	oscMix = lerp(oscMix, targetOscMix, 100 * dt);
	UpdateSplitAndWave();

	// skip processing if voice is silent
	UpdateEnvelopes();
	if (GetVolume() == 0.0 && filter.IsSilent()) return 0.0;

	// mono glide
	if (baseFrequency != targetFrequency)
		baseFrequency = lerp(baseFrequency, targetFrequency, glideSpeed * dt);

	// lfo delay
	lfoValue *= delayEnvelope.Get();

	// main processing
	auto out = GetOscillators(lfoValue, driftValue);
	out *= GetVolume();
	if (filterEnabled)
		out = filter.Process(out, GetFilterCutoff(lfoValue, driftValue));
	out *= fadeVolume;
	return out;
}
