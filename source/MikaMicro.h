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
	Osc2Wave,
	Osc2Coarse,
	Osc2Fine,
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

struct Oscillator
{
	Waveforms waveform = Waveforms::Sine;
	double tune = 1.0;
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
	void UpdateEnvelopes();
	double GetOscillator(Oscillator &osc, double frequency);

	IMidiQueue midiQueue;
	double dt = 0.0;
	std::array<EnvelopeStages, numVoices> volEnvStage;
	std::array<double, numVoices> volEnvValue;
	double volEnvA = 0.0;
	double volEnvD = 0.0;
	double volEnvS = 0.0;
	double volEnvR = 0.0;
	std::array<int, numVoices> note;
	std::array<double, numVoices> frequency;
	std::array<Oscillator, numVoices> osc1;
	std::array<Oscillator, numVoices> osc2;
	double osc1Pitch;
	double osc2Pitch;
};

#endif
