#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(osc1Split)->InitDouble("Oscillator 1 split", 0.0, 0.0, .025, .01);
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
	GetParam(filterKeyTrack)->InitDouble("Filter key tracking", 0.0, 0.0, 1.0, .01);

	// modulation sources
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 0.1, 0.1, 1000.0, .01, "", "", .01);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.1, 1000.0, .01, "", "", .01);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 998.0, 0.1, 1000.0, .01, "", "", .01);

	// modulation targets
	GetParam(volEnvPitch)->InitDouble("Volume envelope to pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(volEnvOsc2)->InitDouble("Volume envelope to oscillator 2 pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(modEnvPitch)->InitDouble("Modulation envelope to pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(modEnvOsc2)->InitDouble("Modulation envelope to oscillator 2 pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(modEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(modEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");

	// master
	GetParam(masterVolume)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);

	for (int i = 0; i < numParameters; i++)
		parameters.push_back(GetParam(i)->Value());
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 55);
	auto smallKnob = pGraphics->LoadIBitmap(SMALLKNOB_ID, SMALLKNOB_FN, 55);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);

	// oscillators
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 4, 12 * 4, osc1Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 4, 12 * 4, osc1Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 74 * 4, 12 * 4, osc1Split, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 4, 28 * 4, osc2Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 4, 28 * 4, osc2Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 74 * 4, 28 * 4, osc2Split, &knob));
	pGraphics->AttachControl(new IFaderControl(this, 96.5 * 4, 17 * 4, 22 * 4, oscMix, &slider));

	// fm
	pGraphics->AttachControl(new IKnobMultiControl(this, 26 * 4, 44 * 4, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 4, 44 * 4, fmFine, &knob));

	// filter
	pGraphics->AttachControl(new IKnobMultiControl(this, 26 * 4, 64 * 4, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 4, 64 * 4, filterRes1, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 4, 64 * 4, filterRes2, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 74 * 4, 64 * 4, filterKeyTrack, &knob));

	// envelopes
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 21 * 4, 22 * 4, volEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 21 * 4, 22 * 4, volEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 21 * 4, 22 * 4, volEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 21 * 4, 22 * 4, volEnvR, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57 * 4, 22 * 4, modEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57 * 4, 22 * 4, modEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57 * 4, 22 * 4, modEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57 * 4, 22 * 4, modEnvR, &slider));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 176.5 * 4, 50.5 * 4, volEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186.5 * 4, 50.5 * 4, volEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 196.5 * 4, 50.5 * 4, volEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 206.5 * 4, 50.5 * 4, volEnvCutoff, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176.5 * 4, 60.5 * 4, modEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186.5 * 4, 60.5 * 4, modEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 196.5 * 4, 60.5 * 4, modEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 206.5 * 4, 60.5 * 4, modEnvCutoff, &smallKnob));

	// master
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 4, 90 * 4, masterVolume, &knob));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();

	MakeDefaultPreset((char *) "-", 1);

	for (int i = 0; i < numVoices; i++) voices.push_back(Voice());
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
		midiReceiver.Process(voices, s);

		double out = 0.0;
		for (auto& voice : voices)
			out += .5 * voice.Next(dt, parameters);
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
			paramIdx == modEnvA || paramIdx == modEnvD || paramIdx == modEnvR)
	{
		parameters[paramIdx] = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - parameters[paramIdx];
	}

	if (paramIdx == osc1Coarse || paramIdx == osc1Fine)
		for (auto &v : voices)
			v.SetOsc1Pitch(parameters[osc1Coarse], parameters[osc1Fine]);
	if (paramIdx == osc2Coarse || paramIdx == osc2Fine)
		for (auto &v : voices)
			v.SetOsc2Pitch(parameters[osc2Coarse], parameters[osc2Fine]);
}