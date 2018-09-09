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
	NumParameters
};

enum class EnvelopeStages
{
	Attack,
	Decay,
	Release,
	Idle,
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
	bool IsReleased(double voice) { return volEnvStage[voice] == EnvelopeStages::Release || volEnvStage[voice] == EnvelopeStages::Idle; }
	void FlushMidi(int s);
	void UpdateEnvelopes();

	IMidiQueue midiQueue;
	double dt = 0.0;
	std::array<EnvelopeStages, numVoices> volEnvStage;
	std::array<double, numVoices> volEnvValue;
	std::array<int, numVoices> note;
	std::array<double, numVoices> frequency;
	std::array<double, numVoices> phase;
};

#endif
