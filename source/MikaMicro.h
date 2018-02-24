#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
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
	filterF,
	filterRes1,
	filterRes2,
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

	void InitParameters();
	void InitGraphics();
	void PlayVoices(int s);
	double GetVoices();
	void GrayOutControls();
};

#endif
