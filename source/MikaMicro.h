#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "SmoothSwitch.h"
#include "Util.h"
#include "Voice.h"

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
	void FlushMidi(int s);
	void UpdateParameters();
	void UpdateEnvelopes();
	double GetVoice(Voice &voice);

	IMidiQueue midiQueue;
	double dt = 0.0;
	std::array<Voice, numVoices> voices;

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
};

#endif
