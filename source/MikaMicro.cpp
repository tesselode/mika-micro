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
	GetParam(oscMix)->InitDouble("Oscillator mix", 0.0, 0.0, 1.0, .01);

	// fm
	GetParam(fmCoarse)->InitInt("FM coarse", 0, -24, 24, "semitones");
	GetParam(fmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01, "semitones");

	// filter
	GetParam(filterCutoff)->InitDouble("Filter cutoff", 20000., 20., 20000., .01, "hz", "", 2.0);
	GetParam(filterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 0.9, .01);
	GetParam(filterDrive)->InitDouble("Filter drive", 0.1, 0.1, 2.0, .01);

	// modulation sources
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 100.0, 0.1, 1000.0, .01, "", "", 10.0);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 10.0, 0.1, 1000.0, .01, "", "", 10.0);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.1, 1000.0, .01, "", "", 10.0);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 10.0, 0.1, 1000.0, .01, "", "", 10.0);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 10.0, 0.1, 1000.0, .01, "", "", 10.0);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 10.0, 0.1, 1000.0, .01, "", "", 10.0);

	// modulation targets
	GetParam(volEnvPitch)->InitDouble("Volume envelope to pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(volEnvOsc2)->InitDouble("Volume envelope to oscillator 2 pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(modEnvPitch)->InitDouble("Modulation envelope to pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(modEnvOsc2)->InitDouble("Modulation envelope to oscillator 2 pitch", 1.0, 0.25, 4.0, .01, "", "", 2.0);
	GetParam(modEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(modEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");

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
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 5, 12 * 5, osc1Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 5, 12 * 5, osc1Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 74 * 5, 12 * 5, osc1Split, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 5, 28 * 5, osc2Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 5, 28 * 5, osc2Fine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 74 * 5, 28 * 5, osc2Split, &knob));
	pGraphics->AttachControl(new IFaderControl(this, 96.5 * 5 + 1, 17 * 5, 22 * 5, oscMix, &slider));

	// fm
	pGraphics->AttachControl(new IKnobMultiControl(this, 26 * 5, 44 * 5, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 5, 44 * 5, fmFine, &knob));

	// filter
	pGraphics->AttachControl(new IKnobMultiControl(this, 26 * 5, 64 * 5, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 5, 64 * 5, filterResonance, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 5, 64 * 5, filterDrive, &knob));

	// envelopes
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 5 + 1, 21 * 5, 22 * 5, volEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 5 + 1, 21 * 5, 22 * 5, volEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 5 + 1, 21 * 5, 22 * 5, volEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 5 + 1, 21 * 5, 22 * 5, volEnvR, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 5 + 1, 57 * 5, 22 * 5, modEnvA, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 5 + 1, 57 * 5, 22 * 5, modEnvD, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 5 + 1, 57 * 5, 22 * 5, modEnvS, &slider));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 5 + 1, 57 * 5, 22 * 5, modEnvR, &slider));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 176.5 * 5, 50.5 * 5, volEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186.5 * 5, 50.5 * 5, volEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 196.5 * 5, 50.5 * 5, volEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 206.5 * 5, 50.5 * 5, volEnvCutoff, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176.5 * 5, 60.5 * 5, modEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186.5 * 5, 60.5 * 5, modEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 196.5 * 5, 60.5 * 5, modEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 206.5 * 5, 60.5 * 5, modEnvCutoff, &smallKnob));

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
			out += .25 * voice.Next(dt, parameters);

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

	if (paramIdx == osc1Coarse || paramIdx == osc1Fine)
		for (auto &v : voices)
			v.SetOsc1Pitch(parameters[osc1Coarse], parameters[osc1Fine]);
	if (paramIdx == osc2Coarse || paramIdx == osc2Fine)
		for (auto &v : voices)
			v.SetOsc2Pitch(parameters[osc2Coarse], parameters[osc2Fine]);
}