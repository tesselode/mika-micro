#pragma once

#include "IParam.h"
#include <functional>

enum class Parameters
{
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

