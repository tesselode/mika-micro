#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Parameter.h"
#include "Voice.h"

enum class Parameters
{
	Volume,
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
	void ProcessMidiMsg(IMidiMsg* message) { midiQueue.Add(message); }

private:
	void InitParameters();
	void InitGraphics();

	IMidiQueue midiQueue;
	std::array<std::unique_ptr<Parameter>, (int)Parameters::NumParameters> parameters;
	double dt = 0.0;
	Voice voice;
};

#endif
