#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include "IPlug_include_in_plug_hdr.h"
#include "MidiReceiver.h"
#include "Oscillator.h"
#include "Parameters.h"
#include <vector>
#include "Voice.h"

const int numVoices = 8;

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
	double dt = 0.0;

	void InitParameters();
	void InitGraphics();
	void InitVoices();

	Oscillator lfo;
	std::vector<double> parameters;
	std::vector<Voice> voices;
	MidiReceiver midiReceiver;
};

#endif
