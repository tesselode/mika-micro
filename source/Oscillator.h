#pragma once

enum class Waveforms
{
	Sine,
	Triangle,
	Saw,
	Square,
	Pulse,
	Noise,
	NumWaveforms
};

struct Oscillator
{
	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;

	double GetWaveform(Waveforms waveform)
	{
		switch (waveform)
		{
		case Waveforms::Sine:
			return sin(phase * twoPi);
		case Waveforms::Triangle:
			triLast = triCurrent;
			triCurrent = phaseIncrement * GeneratePulse(phase, phaseIncrement, .5) + (1.0 - phaseIncrement) * triLast;
			return triCurrent * 5.0;
		case Waveforms::Saw:
			return 1.0 - 2.0 * phase + Blep(phase, phaseIncrement);
			break;
		case Waveforms::Square:
			return GeneratePulse(phase, phaseIncrement, .5);
		case Waveforms::Pulse:
			return GeneratePulse(phase, phaseIncrement, .75);
		case Waveforms::Noise:
			// Ove Karlsen's noise algorithm
			// http://musicdsp.org/showArchiveComment.php?ArchiveID=217
			noiseValue += 19.0;
			noiseValue *= noiseValue;
			noiseValue -= (int)noiseValue;
			return noiseValue - .5;
		}
	}

	double Get(double dt, double frequency)
	{
		phaseIncrement = frequency * dt;
		phase += phaseIncrement;
		while (phase > 1.0) phase -= 1.0;
		return GetWaveform(Waveforms::Sine);
	}

	double Get(double dt, SmoothSwitch &waveform, double frequency)
	{
		phaseIncrement = frequency * dt;
		phase += phaseIncrement;
		while (phase > 1.0) phase -= 1.0;

		switch (waveform.switching)
		{
		case true:
		{
			auto out = 0.0;
			out += (1.0 - waveform.mix) * GetWaveform((Waveforms)(int)waveform.previous);
			out += waveform.mix * GetWaveform((Waveforms)(int)waveform.current);
			return out;
		}
		case false:
			return GetWaveform((Waveforms)(int)waveform.current);
		}
	}
};
