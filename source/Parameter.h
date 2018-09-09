#pragma once

#include "IParam.h"
#include <functional>

const double parameterCloseEnoughThreshold = .001;

enum class PublicParameters
{
	Osc1Wave,
	Osc1Coarse,
	Osc1Fine,
	Osc1Split,
	Osc2Wave,
	Osc2Coarse,
	Osc2Fine,
	Osc2Split,
	OscMix,
	FmMode,
	FmCoarse,
	FmFine,
	FilterMode,
	FilterCutoff,
	FilterResonance,
	FilterKeyTracking,
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	VolEnvV,
	ModEnvA,
	ModEnvD,
	ModEnvS,
	ModEnvR,
	ModEnvV,
	LfoAmount,
	LfoFrequency,
	LfoDelay,
	VolEnvFm,
	VolEnvCutoff,
	ModEnvFm,
	ModEnvCutoff,
	LfoFm,
	LfoCutoff,
	VoiceMode,
	GlideLength,
	Volume,
	NumParameters
};

enum class InternalParameters
{
	Osc1SineMix,
	Osc1TriangleMix,
	Osc1SawMix,
	Osc1SquareMix,
	Osc1PulseMix,
	Osc1NoiseMix,
	Osc1Coarse,
	Osc1Fine,
	Osc1SplitMix,
	Osc1SplitFactorA,
	Osc1SplitFactorB,
	Osc2SineMix,
	Osc2TriangleMix,
	Osc2SawMix,
	Osc2SquareMix,
	Osc2PulseMix,
	Osc2NoiseMix,
	Osc2Coarse,
	Osc2Fine,
	Osc2SplitMix,
	Osc2SplitFactorA,
	Osc2SplitFactorB,
	OscMix,
	FmMode,
	FmCoarse,
	FmFine,
	FilterDryMix,
	Filter2pMix,
	FilterSvfMix,
	Filter4pMix,
	FilterCutoff,
	FilterResonance,
	FilterKeyTracking,
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	VolEnvV,
	ModEnvA,
	ModEnvD,
	ModEnvS,
	ModEnvR,
	ModEnvV,
	LfoAmount,
	LfoFrequency,
	LfoDelay,
	VolEnvFm,
	VolEnvCutoff,
	ModEnvFm,
	ModEnvCutoff,
	LfoFm,
	LfoCutoff,
	VoiceMode,
	GlideLength,
	Volume,
	NumParameters
};

class Parameter
{
public:
	Parameter(IParam* p) : parameter(p) {}
	void SetTransformation(std::function<double(double)> f) { transformation = f; }
	void DisableSmoothing() { smooth = false; }
	void OnParameterChanged();
	void Update(double dt);
	double Get() { return value; }
	IParam* GetParameter() { return parameter; }

private:
	IParam* parameter;
	bool firstSample = true;
	bool upToDate = false;
	double target = 0.0;
	double value = 0.0;
	bool smooth = true;
	std::function<double(double)> transformation = [](double v) { return v; };
};
