#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "PresetMenu.h"
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

	// parameters
	std::array<double, kNumParameters> parameters;
	double oscMix = 0.0;

	// graphics
	IGraphics* pGraphics;

	// midi
	IMidiQueue midiQueue;
	std::vector<int> heldNotes;

	// sample rate
	double dt = 0.0;

	// modulation
	Oscillator lfo;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	
	// voices
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
