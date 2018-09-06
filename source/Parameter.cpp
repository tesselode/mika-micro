#include "Parameter.h"

void Parameter::Update(double dt)
{
	auto target = transformation(parameter->Value());
	switch (smooth)
	{
	case true:
		value += (target - value) * 100.0 * dt;
		break;
	case false:
		value = target;
		break;
	}
}
