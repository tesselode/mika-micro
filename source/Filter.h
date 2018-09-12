#pragma once

#include <cmath>
#include "SmoothSwitch.h"
#include "Util.h"

enum class FilterModes
{
	Off,
	TwoPole,
	Svf,
	FourPole,
	NumFilterModes
};

struct TwoPoleFilter
{
	double a = 0.0;
	double b = 0.0;

	void Reset()
	{
		a = 0.0;
		b = 0.0;
	}

	bool IsSilent() { return b == 0.0; }

	double Process(double dt, double input, double cutoff, double resonance)
	{
		// f calculation
		auto f = 2 * sin(pi * cutoff * dt);
		f = f > .99 ? .99 : f < .01 ? .01 : f;

		// feedback calculation
		auto feedback = resonance + resonance / (1.0 - f);
		feedback = fastAtan(feedback * .1) * 10.0;

		// main processing
		a += f * (input - a + feedback * (a - b));
		a = fastAtan(a * .1) * 10.0;
		b += f * (a - b);
		b = fastAtan(b * .1) * 10.0;

		return b;
	}
};

struct StateVariableFilter
{
	double band = 0.0;
	double low = 0.0;

	void Reset()
	{
		band = 0.0;
		low = 0.0;
	}

	bool IsSilent() { return low == 0.0; }

	double StateVariableFilter::Process(double dt, double input, double cutoff, double resonance)
	{
		// f calculation
		auto f = 2 * sin(pi * cutoff * dt);
		f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

		// resonance rolloff
		auto maxResonance = 1.0 - f * f * f * f * f;
		resonance = resonance > maxResonance ? maxResonance : resonance;

		// main processing
		auto high = input - (low + band * (1.0 - resonance));
		band += f * high;
		low += f * band;
		low = fastAtan(low * .1) * 10.0;

		return low;
	}
};

struct FourPoleFilter
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.0;

	void Reset()
	{
		a = 0.0;
		b = 0.0;
		c = 0.0;
		d = 0.0;
	}

	bool IsSilent() { return d == 0.0; }

	double FourPoleFilter::Process(double dt, double input, double cutoff, double resonance)
	{
		// f calculation
		auto f = 2 * sin(pi * cutoff * dt);
		f = f > .99 ? .99 : f < .01 ? .01 : f;

		// feedback calculation
		auto feedback = resonance + resonance / (1.0 - f);
		feedback = fastAtan(feedback * .1) * 10.0;

		// main processing
		a += f * (input - a + feedback * (a - b));
		a = fastAtan(a * .1) * 10.0;
		b += f * (a - b);
		b = fastAtan(b * .1) * 10.0;
		c += f * (b - c);
		c = fastAtan(c * .1) * 10.0;
		d += f * (c - d);
		d = fastAtan(d * .1) * 10.0;

		return d;
	}
};

struct Filter
{
	TwoPoleFilter twoPoleFilter;
	StateVariableFilter stateVariableFilter;
	FourPoleFilter fourPoleFilter;

	void Reset()
	{
		twoPoleFilter.Reset();
		stateVariableFilter.Reset();
		fourPoleFilter.Reset();
	}

	bool IsSilentIndividual(FilterModes mode)
	{
		switch (mode)
		{
		case FilterModes::Off:
			return true;
		case FilterModes::TwoPole:
			return twoPoleFilter.IsSilent();
		case FilterModes::Svf:
			return stateVariableFilter.IsSilent();
		case FilterModes::FourPole:
			return fourPoleFilter.IsSilent();
		}
	}

	bool IsSilent(SmoothSwitch mode)
	{
		switch (mode.switching)
		{
		case true:
			return IsSilentIndividual((FilterModes)(int)mode.previous) && IsSilentIndividual((FilterModes)(int)mode.current);
		case false:
			return IsSilentIndividual((FilterModes)(int)mode.current);
		}
	}

	double ProcessIndividual(double dt, double input, FilterModes mode, double cutoff, double resonance)
	{
		switch (mode)
		{
		case FilterModes::Off:
			return input;
		case FilterModes::TwoPole:
			return twoPoleFilter.Process(dt, input, cutoff, resonance);
		case FilterModes::Svf:
			return stateVariableFilter.Process(dt, input, cutoff, resonance);
		case FilterModes::FourPole:
			return fourPoleFilter.Process(dt, input, cutoff, resonance);
		}
	}

	double Process(double dt, double input, SmoothSwitch mode, double cutoff, double resonance)
	{
		switch (mode.switching)
		{
		case true:
		{
			auto out = 0.0;
			out += (1.0 - mode.mix) * ProcessIndividual(dt, input, (FilterModes)(int)mode.previous, cutoff, resonance);
			out += mode.mix * ProcessIndividual(dt, input, (FilterModes)(int)mode.current, cutoff, resonance);
			return out;
		}
		case false:
			return ProcessIndividual(dt, input, (FilterModes)(int)mode.current, cutoff, resonance);
		}
	}
};