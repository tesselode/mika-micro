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
	GetParam(fmCoarse)->InitInt("FM coarse", 0, -24, 24);
	GetParam(fmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam(filterF)->InitDouble("Filter cutoff", 1.0, .001, 1.0, .01);
	GetParam(filterRes1)->InitDouble("Filter resonance 1", 0.0, 0.0, 0.9, .01);
	GetParam(filterRes2)->InitDouble("Filter resonance 2", 0.0, 0.0, 0.9, .01);
	GetParam(filterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(modEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(modEnvR)->InitDouble("Modulation envelope release", 998.0, 0.5, 1000.0, .01, "", "", .025);

	// modulation targets
	GetParam(volEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(modEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	//GetParam(lfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(volEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -1.0, 1.0, .01, "hz");
	GetParam(modEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -1.0, 1.0, .01, "hz");
	//GetParam(lfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, 0.0, 20000., .01, "hz", "", 2.0);
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
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
	pGraphics->AttachControl(new IKnobMultiControl(this, 24 * 4, 64 * 4, filterF, &knob));
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
	//pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 22 * 4, 20 * 4, volEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 120.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 119.5 * 4, 58 * 4, 20 * 4, modEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 128.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 127.5 * 4, 58 * 4, 20 * 4, modEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 136.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 135.5 * 4, 58 * 4, 20 * 4, modEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 144.5 * 4, 57 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 143.5 * 4, 58 * 4, 20 * 4, modEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 152.5 * 4, 57 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 151.5 * 4, 58 * 4, 20 * 4, modEnvV, &slider));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 12 * 4, lfoAmount, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 12 * 4, lfoFrequency, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 12 * 4, lfoDelay, &knob));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 52 * 4, volEnvFm, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 52 * 4, modEnvFm, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 52 * 4, lfoFm, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 170 * 4, 68 * 4, volEnvCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 186 * 4, 68 * 4, modEnvCutoff, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 202 * 4, 68 * 4, lfoCutoff, &knob));

	// master
	/*pGraphics->AttachControl(new ISwitchControl(this, 24 * 4, 90 * 4, monoMode, &toggleSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 40 * 4, 90 * 4, glideSpeed, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 56 * 4, 90 * 4, masterVolume, &knob));*/

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();

	MakeDefaultPreset((char *) "-", 1);
}

MikaMicro::~MikaMicro() {}

void MikaMicro::PlayVoices(int s)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > s) break;

		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = message->Velocity();
		bool noteOff = status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && velocity == 0);

		if (noteOff)
		{
			for (auto& voice : voices)
				if (voice.GetNote() == note) voice.Release();
		}
		else if (status == IMidiMsg::kNoteOn)
		{
			// get the quietest voice, prioritizing voices that are released
			auto voice = std::min_element(
				std::begin(voices),
				std::end(voices),
				[](Voice a, Voice b)
			{
				return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
			});
			voice->SetNote(note);
			voice->Start();
		}

		midiQueue.Remove();
	}
}

double MikaMicro::GetVoices()
{
	auto out = 0.0;
	for (auto &voice : voices) out += voice.Next();
	return out * .25;
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		PlayVoices(s);
		auto out = GetVoices();
		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	for (auto &voice : voices) voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	auto value = GetParam(paramIdx)->Value();

	// reverse parameters
	switch (paramIdx)
	{
	case oscMix:
	case filterRes1:
	case filterRes2:
	case volEnvA:
	case volEnvD:
	case volEnvR:
	case modEnvA:
	case modEnvD:
	case modEnvR:
		value = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - value;
	}

	switch (paramIdx)
	{
	case osc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((OscillatorWaveform)(int)value);
		break;
	case osc1Coarse:
	case osc1Fine:
		for (auto &voice : voices)
			voice.SetOsc1Tune(GetParam(osc1Coarse)->Value() + GetParam(osc1Fine)->Value());
		break;
	case osc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(value);
		break;
	case osc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((OscillatorWaveform)(int)value);
		break;
	case osc2Coarse:
	case osc2Fine:
		for (auto &voice : voices)
			voice.SetOsc2Tune(GetParam(osc2Coarse)->Value() + GetParam(osc2Fine)->Value());
		break;
	case osc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(value);
		break;
	case oscMix:
		for (auto &voice : voices) voice.SetOscMix(value);
		break;
	case fmCoarse:
		for (auto &voice : voices) voice.SetFmCoarse(value);
		break;
	case fmFine:
		for (auto &voice : voices) voice.SetFmFine(value);
		break;
	case filterF:
		for (auto &voice : voices) voice.SetFilterF(value);
		break;
	case filterRes1:
		for (auto &voice : voices) voice.SetFilterRes1(value);
		break;
	case filterRes2:
		for (auto &voice : voices) voice.SetFilterRes2(value);
		break;
	case filterKeyTrack:
		for (auto &voice : voices) voice.SetFilterKeyTrack(value);
		break;
	case volEnvA:
		for (auto &voice : voices) voice.SetVolumeEnvelopeAttack(value);
		break;
	case volEnvD:
		for (auto &voice : voices) voice.SetVolumeEnvelopeDecay(value);
		break;
	case volEnvS:
		for (auto &voice : voices) voice.SetVolumeEnvelopeSustain(value);
		break;
	case volEnvR:
		for (auto &voice : voices) voice.SetVolumeEnvelopeRelease(value);
		break;
	case modEnvA:
		for (auto &voice : voices) voice.SetModEnvelopeAttack(value);
		break;
	case modEnvD:
		for (auto &voice : voices) voice.SetModEnvelopeDecay(value);
		break;
	case modEnvS:
		for (auto &voice : voices) voice.SetModEnvelopeSustain(value);
		break;
	case modEnvR:
		for (auto &voice : voices) voice.SetModEnvelopeRelease(value);
		break;
	case volEnvFm:
		for (auto &voice : voices) voice.SetVolumeEnvelopeFm(value);
		break;
	case modEnvFm:
		for (auto &voice : voices) voice.SetModEnvelopeFm(value);
		break;
	case volEnvCutoff:
		for (auto &voice : voices) voice.SetVolumeEnvelopeCutoff(value);
		break;
	case modEnvCutoff:
		for (auto &voice : voices) voice.SetModEnvelopeCutoff(value);
		break;
	}
}

void MikaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	midiQueue.Add(message);
}
