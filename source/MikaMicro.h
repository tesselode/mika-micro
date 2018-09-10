#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Util.h"

const int numVoices = 8;

enum class Parameters
{
	Osc1Wave,
	Osc1Coarse,
	Osc1Fine,
	Osc1Split,
	Osc2Wave,
	Osc2Coarse,
	Osc2Fine,
	Osc2Split,
	OscMix,
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	NumParameters
};

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

enum class EnvelopeStages
{
	Attack,
	Decay,
	Release,
	Idle,
};

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
		previous = current;
		current = value;
		mix = 0.0;
		switching = true;
	}
};

struct Oscillator
{
	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	void ProcessMidiMsg(IMidiMsg *message) { midiQueue.Add(message); }

private:
	void InitParameters();
	void InitGraphics();
	void InitPresets();
	void InitVoices();
	bool IsReleased(double voice) { return volEnvStage[voice] == EnvelopeStages::Release || volEnvStage[voice] == EnvelopeStages::Idle; }
	void FlushMidi(int s);
	void UpdateParameters();
	void UpdateEnvelopes();
	double GetWaveform(Oscillator &osc, Waveforms waveform);
	double GetOscillator(Oscillator &osc, SmoothSwitch &waveform, double frequency);
	double GetVoice(int voice);

	IMidiQueue midiQueue;
	double dt = 0.0;
	std::array<EnvelopeStages, numVoices> volEnvStage;
	std::array<double, numVoices> volEnvValue;
	std::array<int, numVoices> note;
	std::array<double, numVoices> frequency;
	std::array<Oscillator, numVoices> osc1a;
	std::array<Oscillator, numVoices> osc1b;
	std::array<Oscillator, numVoices> osc2a;
	std::array<Oscillator, numVoices> osc2b;

	SmoothSwitch osc1Wave;
	double osc1Tune = 1.0;
	double targetOsc1SplitMix = 0.0;
	double osc1SplitMix = 0.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	SmoothSwitch osc2Wave;
	double osc2Tune = 1.0;
	double targetOsc2SplitMix = 0.0;
	double osc2SplitMix = 0.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double targetOscMix = 0.0;
	double oscMix = 0.0;
	double volEnvA = 0.0;
	double volEnvD = 0.0;
	double volEnvS = 0.0;
	double volEnvR = 0.0;
};

#endif
