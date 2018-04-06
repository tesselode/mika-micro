#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include <cmath>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "PresetMenu.h"
#include <random>
#include <vector>
#include "Voice.h"

enum EParameters
{
	kOsc1Wave,
	kOsc1Coarse,
	kOsc1Fine,
	kOsc1Split,
	kOsc2Wave,
	kOsc2Coarse,
	kOsc2Fine,
	kOsc2Split,
	kOscMix,
	kFmMode,
	kFmCoarse,
	kFmFine,
	kFilterEnabled,
	kFilterCutoff,
	kFilterResonance,
	kFilterKeyTrack,
	kVolEnvA,
	kVolEnvD,
	kVolEnvS,
	kVolEnvR,
	kVolEnvV,
	kModEnvA,
	kModEnvD,
	kModEnvS,
	kModEnvR,
	kModEnvV,
	kLfoAmount,
	kLfoFrequency,
	kLfoDelay,
	kVolEnvFm,
	kVolEnvCutoff,
	kModEnvFm,
	kModEnvCutoff,
	kLfoFm,
	kLfoCutoff,
	kVoiceMode,
	kGlideSpeed,
	kMasterVolume,
	kNumParameters
};

enum EVoiceModes
{
	kVoiceModePoly,
	kVoiceModeMono,
	kVoiceModeLegato,
	kNumVoiceModes
};

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessMidiMsg(IMidiMsg *message);
	void StopNote(EVoiceModes voiceMode, int note);
	void StartNote(EVoiceModes voiceMode, int note, double velocity);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	IGraphics * pGraphics;
	IMidiQueue midiQueue;
	std::vector<int> heldNotes;
	Oscillator lfo;
	std::array<Voice, 8> voices;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<> dist;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	double dt = 0.0;

	IParam* P(int parameter) { return GetParam(parameter); }
	double V(int parameter) { return P(parameter)->Value(); }
	void InitParameters();
	void InitGraphics();
	void InitPresets();
	void FlushMidiQueue(int s);
	double GetDriftValue();
	double GetVoices();
	void GrayOutControls();
};

#endif
