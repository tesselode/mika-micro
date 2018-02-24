#include "Voice.h"

void Voice::Start()
{
	if (GetVolume() == 0.0)
	{
		osc1a.Reset();
		osc1b.Reset();
		osc2a.Reset();
		osc2b.Reset();
		oscFm.Reset();
	}
	volumeEnvelope.Start();
	modEnvelope.Start();
}
void Voice::Release()
{
	volumeEnvelope.Release();
	modEnvelope.Release();
}

double Voice::GetFilterF()
{
	double f = filterF;
	if (volEnvCutoff != 0.0) f += volEnvCutoff * volumeEnvelope.Get();
	if (modEnvCutoff != 0.0) f += modEnvCutoff * modEnvelope.Get();
	if (filterKeyTrack != 0.0) f += filterKeyTrack * baseFrequency * .00005;
	return f;
}

double Voice::GetOscillators()
{
	double out = 0.0;

	double osc1BaseFrequency = baseFrequency * osc1TuneFactor;

	// fm
	double fmFactor = 1.0;
	if (fmCoarse != 0)
	{
		double fmAmount = fabs(fmCoarse) + fmFine;
		if (volEnvFm) fmAmount += volEnvFm * volumeEnvelope.Get();
		if (modEnvFm) fmAmount += modEnvFm * modEnvelope.Get();
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
		double osc2BaseFrequency = baseFrequency * osc2TuneFactor;
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

double Voice::Next()
{
	UpdateEnvelopes();
	if (GetVolume() == 0.0) return 0.0;

	auto out = GetOscillators();
	if (filterF < 1.0)
	{
		auto f = GetFilterF();
		for (int i = 0; i < 2; i++) out = filter.Process(out, f);
	}
	out *= GetVolume();
	return out;
}
