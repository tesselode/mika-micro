#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include <random>
#include <vector>
#include "Voice.h"

enum ParameterList
{
	osc1Wave,
	osc1Coarse,
	osc1Fine,
	osc1Split,
	osc2Wave,
	osc2Coarse,
	osc2Fine,
	osc2Split,
	oscMix,
	fmCoarse,
	fmFine,
	filterEnabled,
	filterF,
	filterRes,
	filterKeyTrack,
	volEnvA,
	volEnvD,
	volEnvS,
	volEnvR,
	volEnvV,
	modEnvA,
	modEnvD,
	modEnvS,
	modEnvR,
	modEnvV,
	lfoAmount,
	lfoFrequency,
	lfoDelay,
	volEnvFm,
	volEnvCutoff,
	modEnvFm,
	modEnvCutoff,
	lfoFm,
	lfoCutoff,
	monoMode,
	glideSpeed,
	masterVolume,
	numParameters
};

struct HeldNote
{
	int note;
	double velocity;
};

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessMidiMsg(IMidiMsg *message);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	IGraphics * pGraphics;
	IMidiQueue midiQueue;
	std::vector<HeldNote> heldNotes;
	Oscillator lfo;
	std::array<Voice, 8> voices;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<> dist;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	double dt = 0.0;

	void InitParameters();
	void InitGraphics();
	void PlayVoices(int s);
	double GetDriftValue();
	double GetVoices();
	void GrayOutControls();
};

#endif
