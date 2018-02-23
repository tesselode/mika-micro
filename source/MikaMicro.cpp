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
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(lfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(lfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(lfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(modEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(lfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(modEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -20000., 20000., .01, "hz");
	GetParam(lfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, 0.0, 20000., .01, "hz", "", 2.0);

	// master
	GetParam(monoMode)->InitBool("Mono mode", true);
	GetParam(glideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(masterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);
}

void MikaMicro::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 55);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto sliderBg = pGraphics->LoadIBitmap(SLIDERBG_ID, SLIDERBG_FN, 1);
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
	pGraphics->AttachControl(new IBitmapControl(this, 93.5 * 4, 17 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 92.5 * 4, 18 * 4, 20 * 4, oscMix, &slider));

	// fm
	pGraphics->AttachControl(new IKnobMultiControl(this, 24 * 4, 44 * 4, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 44 * 4, fmFine, &knob));

	// filter
	pGraphics->AttachControl(new IKnobMultiControl(this, 24 * 4, 64 * 4, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 64 * 4, filterRes1, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 64 * 4, filterRes2, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 72 * 4, 64 * 4, filterKeyTrack, &knob));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 120.5 * 4, 21 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 119.5 * 4, 22 * 4, 20 * 4, volEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 128.5 * 4, 21 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 127.5 * 4, 22 * 4, 20 * 4, volEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 136.5 * 4, 21 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 135.5 * 4, 22 * 4, 20 * 4, volEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 144.5 * 4, 21 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 143.5 * 4, 22 * 4, 20 * 4, volEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 152.5 * 4, 21 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 22 * 4, 20 * 4, volEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 120.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 119.5 * 4, 58 * 4, 20 * 4, modEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 128.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 127.5 * 4, 58 * 4, 20 * 4, modEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 136.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 135.5 * 4, 58 * 4, 20 * 4, modEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 144.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 143.5 * 4, 58 * 4, 20 * 4, modEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 152.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 58 * 4, 20 * 4, modEnvV, &slider));
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
		voices.push_back(Voice(parameters));
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 128, instanceInfo)
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
			out += voice.Next(dt, lfoValue);
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

void MikaMicro::GrayOutControls()
{
	auto fmEnabled = (parameters[fmCoarse] < 0.0 && parameters[oscMix] < 1.0) ||
		(parameters[fmCoarse] > 0.0 && parameters[oscMix] > 0.0);
	auto osc1Enabled = parameters[oscMix] < 1.0;
	auto osc2Enabled = parameters[oscMix] > 0.0;
	auto filterEnabled = parameters[filterCutoff] < 20000.;
	auto modEnvEnabled = parameters[modEnvFm] != 0.0 || parameters[modEnvCutoff] != 0.0;
	auto vibratoEnabled = parameters[lfoFm] != 0.0 || parameters[lfoCutoff] != 0.0 ||
		parameters[lfoAmount] < 0.0 || (parameters[lfoAmount] > 0.0 && osc2Enabled);
	
	pGraphics->GetControl(1)->GrayOut(!osc1Enabled);
	pGraphics->GetControl(2)->GrayOut(!(osc1Enabled || fmEnabled));
	pGraphics->GetControl(3)->GrayOut(!(osc1Enabled || fmEnabled));
	pGraphics->GetControl(4)->GrayOut(!osc1Enabled);
	for (int i = 5; i < 9; i++) pGraphics->GetControl(i)->GrayOut(!osc2Enabled);
	pGraphics->GetControl(12)->GrayOut(!fmEnabled);
	for (int i = 14; i < 17; i++) pGraphics->GetControl(i)->GrayOut(!filterEnabled);
	for (int i = 27; i < 37; i++) pGraphics->GetControl(i)->GrayOut(!modEnvEnabled);
	for (int i = 38; i < 40; i++) pGraphics->GetControl(i)->GrayOut(!vibratoEnabled);
	for (int i = 40; i < 43; i++) pGraphics->GetControl(i)->GrayOut(!fmEnabled);
	for (int i = 43; i < 46; i++) pGraphics->GetControl(i)->GrayOut(!filterEnabled);
	pGraphics->GetControl(47)->GrayOut(!parameters[monoMode]);
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	parameters[paramIdx] = GetParam(paramIdx)->Value();

	switch (paramIdx)
	{
	// reverse parameters
	case oscMix:
	case volEnvA:
	case volEnvD:
	case volEnvR:
	case modEnvA:
	case modEnvD:
	case modEnvR:
	case lfoDelay:
	case glideSpeed:
		parameters[paramIdx] = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - parameters[paramIdx];
		break;

	// oscillator pitch
	case osc1Coarse:
	case osc1Fine:
		for (auto &v : voices) v.SetOsc1Pitch(parameters[osc1Coarse], parameters[osc1Fine]);
		break;
	case osc2Coarse:
	case osc2Fine:
		for (auto &v : voices) v.SetOsc2Pitch(parameters[osc2Coarse], parameters[osc2Fine]);
		break;

	// mono
	case monoMode:
		midiReceiver.SetMono((bool)parameters[monoMode]);
		for (auto &v : voices) v.Release();
		break;
	}

	GrayOutControls();
}