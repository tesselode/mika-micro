#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Voice.h"

enum ParameterList
{
	osc1Coarse,
	osc1Fine,
	osc1Split,
	osc2Coarse,
	osc2Fine,
	osc2Split,
	fmCoarse,
	fmFine,
	filterF,
	filterRes1,
	filterRes2,
	volEnvA,
	volEnvD,
	volEnvS,
	volEnvR,
	numParameters,
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
	IMidiQueue midiQueue;
	std::array<Voice, 8> voices;

	void InitParameters();
	void InitGraphics();
	void PlayVoices(int s);
	double GetVoices();
};

#endif
