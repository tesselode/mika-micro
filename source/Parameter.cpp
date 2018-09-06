#include "Parameter.h"

void Parameter::Update(double dt)
{
	auto rawValue = parameter->Value();
	if (rawValue != previousRawValue) upToDate = false;
	if (!upToDate)
	{
		target = transformation(parameter->Value());
		upToDate = true;
	}
	previousRawValue = rawValue;

	if (value != target)
	{
		if (smooth)
		{
			value += (target - value) * 100.0 * dt;
			if (abs(target - value) < parameterCloseEnoughThreshold)
				value = target;
		}
		else
			value = target;
	}
}
