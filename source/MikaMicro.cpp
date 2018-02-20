#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(osc1Wave)->InitEnum("Oscillator 1 waveform", OscillatorWaveformSaw, numWaveforms);
	GetParam(osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(osc1Split)->InitDouble("Oscillator 1 split", 0.0, 0.0, .025, .01);
	GetParam(osc2Wave)->InitEnum("Oscillator 2 waveform", OscillatorWaveformSaw, numWaveforms);
	GetParam(osc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam(osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(osc2Split)->InitDouble("Oscillator 2 split", 0.0, 0.0, .025, .01);
	GetParam(oscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam(fmCoarse)->InitInt("FM coarse", 0, -24, 24, "semitones");
	GetParam(fmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01, "semitones");

	// filter
	GetParam(filterCutoff)->InitDouble("Filter cutoff", 20000., 20., 20000., .01, "hz", "", 2.0);
	GetParam(filterRes1)->InitDouble("Filter resonance 1", 0.0, 0.0, 0.9, .01);
	GetParam(filterRes2)->InitDouble("Filter resonance 2", 0.0, 0.0, 0.9, .01);
	GetParam(filterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 0.1, 0.1, 1000.0, .01, "", "", .01);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(volEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(lfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(lfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(lfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(modEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(modEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(lfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(lfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -20000., 20000., .01, "hz");

	// master
	GetParam(monoMode)->InitBool("Mono mode", true);
	GetParam(glideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(masterVolume)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);

	for (int i = 0; i < numParameters; i++)
		parameters.push_back(GetParam(i)->Value());
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 55);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, numWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);

	// oscillators
	pGraphics->AttachControl(new ISwitchControl(this, 24 * 4, 12 * 4, osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 12 * 4, osc1Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 12 * 4, osc1Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 72 * 4, 12 * 4, osc1Split, &knob));
	pGraphics->AttachControl(new ISwitchControl(this, 24 * 4, 28 * 4, osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 28 * 4, osc2Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 28 * 4, osc2Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 72 * 4, 28 * 4, osc2Split, &knob));
	pGraphics->AttachControl(new IFaderControl(this, 92.5 * 4, 17 * 4, 22 * 4, oscMix, &slider));

	// fm
	pGraphics->AttachControl(new IKnobMultiControl(this, 24 * 4, 44 * 4, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 44 * 4, fmFine, &knob));

	// filter
	pGraphics->AttachControl(new IKnobMultiControl(this, 24 * 4, 64 * 4, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 64 * 4, filterRes1, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 64 * 4, filterRes2, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 72 * 4, 64 * 4, filterKeyTrack, &knob));

	// modulation sources
	pGraphics->AttachControl(new IFaderControl(this, 119.5 * 4, 21 * 4, 22 * 4, volEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 127.5 * 4, 21 * 4, 22 * 4, volEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 135.5 * 4, 21 * 4, 22 * 4, volEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 143.5 * 4, 21 * 4, 22 * 4, volEnvR, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 21 * 4, 22 * 4, volEnvV, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 119.5 * 4, 57 * 4, 22 * 4, modEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 127.5 * 4, 57 * 4, 22 * 4, modEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 135.5 * 4, 57 * 4, 22 * 4, modEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 143.5 * 4, 57 * 4, 22 * 4, modEnvR, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 57 * 4, 22 * 4, modEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 12 * 4, lfoAmount, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 12 * 4, lfoFrequency, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 12 * 4, lfoDelay, &knob));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 52 * 4, volEnvFm, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 52 * 4, modEnvFm, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 52 * 4, lfoFm, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 68 * 4, volEnvCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 68 * 4, modEnvCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 68 * 4, lfoCutoff, &knob));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 24 * 4, 90 * 4, monoMode, &toggleSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 90 * 4, glideSpeed, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 90 * 4, masterVolume, &knob));

	AttachGraphics(pGraphics);
}

void MikaMicro::InitVoices()
{
	for (int i = 0; i < numVoices; i++)
		voices.push_back(Voice(parameters, i));
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitVoices();
	InitGraphics();

	MakeDefaultPreset((char *) "-", 1);
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	midiReceiver.Add(message);
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		auto lfoValue = lfo.Next(dt, parameters[lfoFrequency], OscillatorWaveformSine);

		midiReceiver.Process(voices, s);

		double out = 0.0;
		for (auto& voice : voices)
			out += .5 * voice.Next(dt, lfoValue);
		out *= parameters[masterVolume];

		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	parameters[paramIdx] = GetParam(paramIdx)->Value();

	if (paramIdx == oscMix || paramIdx == volEnvA || paramIdx == volEnvD || paramIdx == volEnvR ||
			paramIdx == modEnvA || paramIdx == modEnvD || paramIdx == modEnvR || paramIdx == lfoDelay ||
			paramIdx == glideSpeed)
	{
		parameters[paramIdx] = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - parameters[paramIdx];
	}

	if (paramIdx == osc1Coarse || paramIdx == osc1Fine)
		for (auto &v : voices)
			v.SetOsc1Pitch(parameters[osc1Coarse], parameters[osc1Fine]);
	if (paramIdx == osc2Coarse || paramIdx == osc2Fine)
		for (auto &v : voices)
			v.SetOsc2Pitch(parameters[osc2Coarse], parameters[osc2Fine]);

	if (paramIdx == monoMode)
	{
		midiReceiver.SetMono(GetParam(paramIdx)->Value());
		for (int i = 1; i < std::size(voices); i++)
			voices[i].Release();
	}
}