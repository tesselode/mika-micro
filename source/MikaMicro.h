#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "Envelope.h"
#include "Filter.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "SmoothSwitch.h"
#include "Util.h"
#include <vector>
#include "Voice.h"

const int numVoices = 8;

enum class FmModes
{
	Off,
	Osc1,
	Osc2,
	NumFmModes
};

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
	FmMode,
	FmCoarse,
	FmFine,
	FilterMode,
	FilterCutoff,
	FilterResonance,
	FilterKeyTracking,
	VolEnvA,
	VolEnvD,
	VolEnvS,
	VolEnvR,
	VolEnvV,
	ModEnvA,
	ModEnvD,
	ModEnvS,
	ModEnvR,
	ModEnvV,
	LfoAmount,
	LfoFrequency,
	LfoDelay,
	VolEnvFm,
	VolEnvCutoff,
	ModEnvFm,
	ModEnvCutoff,
	LfoFm,
	LfoCutoff,
	VoiceMode,
	GlideLength,
	MasterVolume,
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
	void UpdateDrift();
	double GetVoice(Voice &voice);
	void GrayOutControls();

	IGraphics* pGraphics;
	IMidiQueue midiQueue;
	std::vector<int> heldNotes;
	double dt = 0.0;
	std::array<Voice, numVoices> voices;
	Oscillator lfo;
	double lfoValue = 0.0;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	double driftValue = 0.0;

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
	double baseFmAmount = 0.0;
	SmoothSwitch filterMode;
	double targetFilterCutoff = 0.0;
	double filterCutoff = 0.0;
	double targetFilterResonance = 0.0;
	double filterResonance = 0.0;
	double targetFilterKeyTracking = 0.0;
	double filterKeyTracking = 0.0;
	double lfoToCutoff = 0.0;
	double glideLength = 0.0;
	double targetMasterVolume = 0.0;
	double masterVolume = 0.0;
};

#endif
