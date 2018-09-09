#include "Parameter.h"

void Parameter::OnParameterChanged()
{
	target = transformation(parameter->Value());
	if (smooth && !firstSample)
		upToDate = false;
	else
	{
		value = target;
		upToDate = true;
	}
}

void Parameter::Update(double dt)
{
	switch (upToDate)
	{
	case false:
		value += (target - value) * 100.0 * dt;
		if (abs(target - value) < parameterCloseEnoughThreshold)
		{
			value = target;
			upToDate = true;
		}
		break;
	}

	firstSample = false;
}
