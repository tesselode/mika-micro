#pragma once

struct SmoothSwitch
{
	double current = -1.0;
	double previous = -1.0;
	double mix = 1.0;
	bool switching = false;

	void Update(double dt)
	{
		switch (switching)
		{
		case true:
			mix += (1.0 - mix) * 100.0 * dt;
			if (mix > .999)
			{
				mix = 1.0;
				switching = false;
			}
		}
	}

	void Switch(double value)
	{
		if (current == value) return;
		previous = current;
		current = value;
		mix = 0.0;
		switching = true;
	}
};