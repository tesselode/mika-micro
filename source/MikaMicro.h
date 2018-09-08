#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Parameter.h"
#include "Util.h"
#include <vector>
#include "Voice.h"

const int numVoices = 8;

enum class VoiceModes
{
	Poly,
	Mono,
	Legato,
	NumVoiceModes
};

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	void ProcessMidiMsg(IMidiMsg* message) { midiQueue.Add(message); }

private:
	void InitParameters();
	void InitGraphics();
	void InitPresets();
	void InitVoices();
	void FlushMidi(int s);

	IMidiQueue midiQueue;
	std::vector<int> heldNotes;
	std::array<std::unique_ptr<Parameter>, (int)InternalParameters::NumParameters> parameters;
	double dt = 0.0;
	std::vector<Voice> voices;
};

#endif
