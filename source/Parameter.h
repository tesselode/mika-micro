#pragma once

#include "IParam.h"

class Parameter
{
public:
	Parameter(IParam* p) : parameter(p) {}
	void Update(double dt)
	{
		auto target = parameter->Value();
		value += (target - value) * 100.0 * dt;
	}
	double Get() { return value; }

private:
	IParam* parameter;
	double value = 0.0;
};

