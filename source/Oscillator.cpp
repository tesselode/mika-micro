#include "Oscillator.h"

double Oscillator::Next(double dt, double frequency, Waveforms waveform)
{
	phase += frequency * dt;
	while (phase > 1.0) phase -= 1.0;

	switch (waveform)
	{
	case Waveforms::Sine:
		return sin(phase * twoPi);
	}
}