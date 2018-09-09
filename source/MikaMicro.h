#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Util.h"

const int numVoices = 8;

enum class Parameters
{
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
	void FlushMidi(int s);

	IMidiQueue midiQueue;
	double dt = 0.0;
	std::array<bool, numVoices> isPlaying;
	std::array<int, numVoices> note;
	std::array<double, numVoices> frequency;
	std::array<double, numVoices> phase;
};

#endif
