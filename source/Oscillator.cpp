#include "Oscillator.h"

double Oscillator::Next(double frequency, OscillatorWaveform waveform)
{
	phase += frequency * dt;
	while (phase >= 1) phase -= 1;
	
	switch (waveform)
	{
	case OscillatorWaveformSine:
		return sin(phase * twoPi);
	}
}
