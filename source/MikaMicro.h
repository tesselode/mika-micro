#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "Voice.h"

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
	void FlushMidi(int sample);
	void GrayOutControls();

	std::array<double, kNumParameters> parameters;
	IMidiQueue midiQueue;
	double dt = 0.0;
	IGraphics* pGraphics;
	std::array<Voice, 8> voices = {
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters)
	};
};

#endif
