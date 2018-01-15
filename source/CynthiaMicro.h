#ifndef __CYNTHIAMICRO__
#define __CYNTHIAMICRO__

#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Voice.h"

const int numVoices = 8;

class CynthiaMicro : public IPlug
{
public:
	CynthiaMicro(IPlugInstanceInfo instanceInfo);
	~CynthiaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	void ProcessMidiMsg(IMidiMsg* message);

private:
	int GetQuietestVoice(bool releasedOnly);
	int PickVoice();

	IGraphics* pGraphics;
	IMidiQueue midiQueue;
	bool mono = false;
	double masterVolume = 0.5;
	std::vector<int> heldNotes;
	std::vector<double> velocities;
	Oscillator lfo;
	Voice voices[numVoices];
};

#endif
