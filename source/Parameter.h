#pragma once

#include "IParam.h"
#include <functional>

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
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	VolEnvV,
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
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	VolEnvV,
	Volume,
	NumParameters
};

class Parameter
{
public:
	Parameter(IParam* p) : parameter(p) {}
	void SetTransformation(std::function<double(double)> f) { transformation = f; }
	void Update(double dt)
	{
		auto target = transformation(parameter->Value());
		value += (target - value) * 100.0 * dt;
	}
	double Get() { return value; }

private:
	IParam* parameter;
	double value = 0.0;
	std::function<double(double)> transformation = [](double v) { return v; };
};
