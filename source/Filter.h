#pragma once

#include <cmath>
#include "Util.h"

enum class FilterModes
{
	Off,
	TwoPole,
	StateVariable,
	FourPole,
	NumFilterModes
};

class TwoPoleFilter
{
public:
	void Reset()
	{
		a = 0.0;
		b = 0.0;
	}
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double a = 0.0;
	double b = 0.0;
};

class StateVariableFilter
{
public:
	void Reset()
	{
		band = 0.0;
		low = 0.0;
	}
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double band = 0.0;
	double low = 0.0;
};

class FourPoleFilter
{
public:
	void Reset()
	{
		a = 0.0;
		b = 0.0;
		c = 0.0;
		d = 0.0;
	}
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.0;
};