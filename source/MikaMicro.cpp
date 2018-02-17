#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(osc1Coarse)->InitInt("Oscillator 1 coarse", -12.0, -24.0, 24.0, "semitones");
	GetParam(osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(osc1Split)->InitDouble("Oscillator 1 split", 0.0, 0.0, .025, .01);
	GetParam(osc2Coarse)->InitInt("Oscillator 2 coarse", 0.0, -24.0, 24.0, "semitones");
	GetParam(osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(osc2Split)->InitDouble("Oscillator 2 split", 0.01, 0.0, .025, .01);
	GetParam(oscMix)->InitDouble("Oscillator mix", 0.5, 0.0, 1.0, .01);

	// fm
	GetParam(fmCoarse)->InitInt("FM coarse", 0, -24, 24, "semitones");
	GetParam(fmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01, "semitones");

	// filter
	GetParam(filterCutoff)->InitDouble("Filter cutoff", 20000., 20., 20000., .01, "hz");
	GetParam(filterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 0.9, .01);
	GetParam(filterDrive)->InitDouble("Filter drive", 0.1, 0.1, 2.0, .01);

	// modulation sources
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 1.0, 0.1, 100.0, .01);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 1.0, 0.1, 100.0, .01);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 1.0, 0.1, 100.0, .01);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 10.0, 0.1, 100.0, .01);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 10.0, 0.1, 100.0, .01);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.75, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 10.0, 0.1, 100.0, .01);

	// modulation targets
	GetParam(volEnvPitch)->InitDouble("Volume envelope to pitch", 1.0, 0.5, 2.0, .01);
	GetParam(volEnvOsc2)->InitDouble("Volume envelope to oscillator 2 pitch", 1.0, 0.5, 2.0, .01);
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(modEnvPitch)->InitDouble("Modulation envelope to pitch", 0.5, 0.5, 2.0, .01);
	GetParam(modEnvOsc2)->InitDouble("Modulation envelope to oscillator 2 pitch", 2.0, 0.5, 2.0, .01);
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
	pGraphics->AttachControl(new IKnobMultiControl(this, 42 * 4, 64 * 4, filterResonance, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 58 * 4, 64 * 4, filterDrive, &knob));

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
	pGraphics->AttachControl(new IKnobMultiControl(this, 178 * 4, 52 * 4, volEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 188 * 4, 52 * 4, volEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 198 * 4, 52 * 4, volEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 208 * 4, 52 * 4, volEnvCutoff, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 178 * 4, 62 * 4, modEnvPitch, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 188 * 4, 62 * 4, modEnvOsc2, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 198 * 4, 62 * 4, modEnvFm, &smallKnob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 208 * 4, 62 * 4, modEnvCutoff, &smallKnob));

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