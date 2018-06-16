#pragma once

#include <array>
#include <cmath>
#include "Util.h"

enum class FilterModes
{
	off,
	twoPole,
	stateVariable,
	numFilterModes
};

// passthrough filter
class Filter
{
public:
	virtual bool IsSilent() { return value == 0.0; }
	virtual double Process(double dt, double input, double cutoff, double resonance)
	{
		value = input;
		return value;
	}

private:
	double value = 0.0;
};

class TwoPoleFilter : public Filter
{
public:
	bool IsSilent() { return b == 0.0; }
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double a = 0.0;
	double b = 0.0;
};

class StateVariableFilter : public Filter
{
public:
	bool IsSilent() { return low == 0.0; }
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double band = 0.0;
	double low = 0.0;
};

class MultiFilter : Filter
{
public:
	void SetMode(FilterModes mode);
	bool IsSilent() { return value == 0.0; }
	double Process(double dt, double input, double cutoff, double resonance);

private:
	std::array<std::shared_ptr<Filter>, (int)FilterModes::numFilterModes> filters = {
		std::shared_ptr<Filter>(new Filter()),
		std::shared_ptr<Filter>(new TwoPoleFilter()),
		std::shared_ptr<Filter>(new StateVariableFilter())
	};

	FilterModes currentMode = FilterModes::twoPole;
	FilterModes previousMode = FilterModes::off;
	bool crossfading = false;
	double currentModeMix = 1.0;

	double value = 0.0;
};