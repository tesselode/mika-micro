#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include <random>
#include <vector>
#include "Voice.h"

enum EVoiceModes
{
	kPoly,
	kMono,
	kLegato,
	kNumVoiceModes
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
	void InitPresets();
	void InitGraphics();
	void FlushMidi(int sample);
	double GetDriftValue();
	void SmoothParameters();
	void GrayOutControls();

	std::array<double, kNumParameters> parameters;
	IMidiQueue midiQueue;
	std::vector<int> heldNotes;
	double dt = 0.0;
	IGraphics* pGraphics;
	Oscillator lfo;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<> dist;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
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
	double oscMix = 0.0;
};

#endif
