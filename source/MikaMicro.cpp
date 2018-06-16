#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(kOsc1Wave)->InitEnum("Oscillator 1 waveform", kSaw, kNumWaveforms);
	GetParam(kOsc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	GetParam(kOsc2Wave)->InitEnum("Oscillator 2 waveform", kSaw, kNumWaveforms);
	GetParam(kOsc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	GetParam(kOscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam(kFmMode)->InitEnum("FM mode", 0, 3);
	GetParam(kFmCoarse)->InitInt("FM coarse", 0, 0, 48);
	GetParam(kFmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam(kFilterMode)->InitEnum("Filter mode", (int)FilterStates::off, (int)FilterStates::numFilterStates);
	GetParam(kFilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam(kFilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam(kFilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(kVolEnvA)->InitDouble("Volume envelope attack", 0.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kVolEnvD)->InitDouble("Volume envelope decay", 1.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kVolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(kVolEnvR)->InitDouble("Volume envelope release", 0.85, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kVolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kModEnvA)->InitDouble("Modulation envelope attack", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kModEnvD)->InitDouble("Modulation envelope decay", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(kModEnvR)->InitDouble("Modulation envelope release", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(kModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kLfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(kLfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(kLfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(kVolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kLfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kVolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(kModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(kLfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	// master
	GetParam(kVoiceMode)->InitEnum("Voice mode", kLegato, kNumVoiceModes);
	GetParam(kGlideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(kMasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);

	// initialize smoothed parameters
	parameters[kOscMix] = GetParam(kOscMix)->GetMin() + GetParam(kOscMix)->GetMax() - GetParam(kOscMix)->Value();
}

void MikaMicro::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 100);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 100);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 100);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto sliderBg = pGraphics->LoadIBitmap(SLIDERBG_ID, SLIDERBG_FN, 1);
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, kNumWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);
	auto fmModeSwitch = pGraphics->LoadIBitmap(FMMODESWITCH_ID, FMMODESWITCH_FN, 3);

	// oscillators
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, kOsc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, kOsc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, kOsc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, kOsc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, kOsc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, kOsc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, kOsc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, kOsc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, kOscMix, &slider));

	// fm
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, kFmMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, kFmCoarse, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, kFmFine, &knobMiddle));

	// filter
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 62 * 4, kFilterMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, kFilterCutoff, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, kFilterResonance, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, kFilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, kVolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, kVolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, kVolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, kVolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, kVolEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, kModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, kModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, kModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, kModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, kModEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, kLfoAmount, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, kLfoFrequency, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, kLfoDelay, &knobLeft));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, kVolEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, kModEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, kLfoFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, kVolEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, kModEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, kLfoCutoff, &knobMiddle));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, kVoiceMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, kGlideSpeed, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, kMasterVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParameters, 128, instanceInfo)
{
	TRACE;

	InitParameters();
	InitPresets();
	InitGraphics();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::FlushMidi(int sample)
{
	while (!midiQueue.Empty())
	{
		auto message = midiQueue.Peek();
		if (message->mOffset > sample) break;

		auto voiceMode = (EVoiceModes)(int)parameters[kVoiceMode];
		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = pow(message->Velocity() * .0078125, 1.25);

		if (status == IMidiMsg::kNoteOn && velocity == 0) status = IMidiMsg::kNoteOff;

		switch (status)
		{
		case IMidiMsg::kNoteOff:
			heldNotes.erase(
				std::remove(
					std::begin(heldNotes),
					std::end(heldNotes),
					note
				),
				std::end(heldNotes)
			);

			switch (voiceMode)
			{
			case kPoly:
				for (auto &voice : voices)
					if (voice.GetNote() == note) voice.Release();
				break;
			case kMono:
			case kLegato:
				if (heldNotes.empty())
					voices[0].Release();
				else
					voices[0].SetNote(heldNotes.back());
				break;
			}
			break;
		case IMidiMsg::kNoteOn:
			switch (voiceMode)
			{
			case kPoly:
			{
				// get the quietest voice, prioritizing voices that are released
				auto voice = std::min_element(
					std::begin(voices),
					std::end(voices),
					[](Voice a, Voice b)
				{
					return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
				}
				);
				voice->SetNote(note);
				voice->SetVelocity(velocity);
				voice->ResetPitch();
				voice->Start();
				break;
			}
			case kMono:
				voices[0].SetNote(note);
				voices[0].SetVelocity(velocity);
				voices[0].Start();
				break;
			case kLegato:
				voices[0].SetNote(note);
				if (heldNotes.empty())
				{
					voices[0].SetVelocity(velocity);
					voices[0].ResetPitch();
					voices[0].Start();
				}
				break;
			}

			heldNotes.push_back(note);
			break;
		case IMidiMsg::kPitchWheel:
		{
			auto pitchBendFactor = pitchFactor(message->PitchWheel() * 2.0);
			for (auto &voice : voices) voice.SetPitchBendFactor(pitchBendFactor);
			break;
		}
		case IMidiMsg::kAllNotesOff:
			for (auto &voice : voices) voice.Release();
			break;
		}

		midiQueue.Remove();
	}
}

double MikaMicro::GetDriftValue()
{
	driftVelocity += random() * 10000.0 * dt;
	driftVelocity -= driftVelocity * 2.0 * dt;
	driftPhase += driftVelocity * dt;
	return .001 * sin(driftPhase);
}

void MikaMicro::SmoothParameters()
{
	parameters[kOscMix] += (oscMix - parameters[kOscMix]) * 100.0 * dt;
	parameters[kFilterCutoff] += (GetParam(kFilterCutoff)->Value() - parameters[kFilterCutoff]) * 100.0 * dt;
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		SmoothParameters();
		auto lfoValue = lfo.Next(dt, parameters[kLfoFrequency]);
		auto driftValue = GetDriftValue();
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Next(dt, lfoValue, driftValue);
		out *= parameters[kMasterVolume];
		outputs[0][s] = outputs[1][s] = out;
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
	auto osc1Enabled = GetParam(kOscMix)->Value() > 0.0;
	auto osc2Enabled = GetParam(kOscMix)->Value() < 1.0;
	auto osc1Noise = (EWaveforms)(int)GetParam(kOsc1Wave)->Value() == kNoise;
	auto osc2Noise = (EWaveforms)(int)GetParam(kOsc2Wave)->Value() == kNoise;
	auto fmEnabled = (GetParam(kFmMode)->Value() == 1 && osc1Enabled && !osc1Noise) ||
		(GetParam(kFmMode)->Value() == 2 && osc2Enabled && !osc2Noise);
	auto filterEnabled = GetParam(kFilterMode)->Value();
	auto modEnvEnabled = GetParam(kModEnvFm)->Value() != 0.0 || GetParam(kModEnvCutoff)->Value() != 0.0;
	auto vibratoEnabled = GetParam(kLfoFm)->Value() != 0.0 || GetParam(kLfoCutoff)->Value() != 0.0 ||
		GetParam(kLfoAmount)->Value() < 0.0 || (GetParam(kLfoAmount)->Value() > 0.0 && osc2Enabled);

	// oscillator 1
	pGraphics->GetControl(1)->GrayOut(!osc1Enabled);
	pGraphics->GetControl(2)->GrayOut(!((osc1Enabled && !osc1Noise) || fmEnabled));
	pGraphics->GetControl(3)->GrayOut(!((osc1Enabled && !osc1Noise) || fmEnabled));
	pGraphics->GetControl(4)->GrayOut(!(osc1Enabled && !osc1Noise));

	// oscillator 2
	pGraphics->GetControl(5)->GrayOut(!osc2Enabled);
	for (int i = 6; i < 9; i++) pGraphics->GetControl(i)->GrayOut(!(osc2Enabled && !osc2Noise));

	// fm
	for (int i = 12; i < 14; i++) pGraphics->GetControl(i)->GrayOut(!fmEnabled);
	for (int i = 41; i < 44; i++) pGraphics->GetControl(i)->GrayOut(!fmEnabled);

	// filter
	for (int i = 15; i < 18; i++) pGraphics->GetControl(i)->GrayOut(!filterEnabled);
	for (int i = 44; i < 47; i++) pGraphics->GetControl(i)->GrayOut(!filterEnabled);

	// mod sources
	for (int i = 28; i < 38; i++) pGraphics->GetControl(i)->GrayOut(!modEnvEnabled);
	for (int i = 39; i < 41; i++) pGraphics->GetControl(i)->GrayOut(!vibratoEnabled);

	// glide
	pGraphics->GetControl(48)->GrayOut(!GetParam(kVoiceMode)->Value());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	// custom curves
	case kVolEnvA:
	case kVolEnvD:
	case kVolEnvR:
	case kModEnvA:
	case kModEnvD:
	case kModEnvR:
		parameters[paramIdx] = 1000 - 999.9 * cosCurve(GetParam(paramIdx)->Value());
		break;
	// reversed parameters
	case kLfoDelay:
	case kGlideSpeed:
		parameters[paramIdx] = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	// curved parameters
	case kLfoCutoff:
	{
		auto v = GetParam(paramIdx)->Value();
		parameters[paramIdx] = copysign((v * .000125) * (v * .000125) * 8000.0, v);
		break;
	}
	// smoothed parameters
	case kOscMix:
		oscMix = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	case kFilterCutoff:
		break;
	// normal parameters
	default:
		parameters[paramIdx] = GetParam(paramIdx)->Value();
		break;
	}

	switch (paramIdx)
	{
	case kOsc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((EWaveforms)(int)parameters[kOsc1Wave]);
		break;
	case kOsc1Coarse:
	case kOsc1Fine:
	{
		auto osc1PitchFactor = pitchFactor(parameters[kOsc1Coarse] + parameters[kOsc1Fine]);
		for (auto &voice : voices) voice.SetOsc1PitchFactor(osc1PitchFactor);
		break;
	}
	case kOsc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(parameters[kOsc1Split]);
		break;
	case kOsc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((EWaveforms)(int)parameters[kOsc2Wave]);
		break;
	case kOsc2Coarse:
	case kOsc2Fine:
	{
		auto osc2PitchFactor = pitchFactor(parameters[kOsc2Coarse] + parameters[kOsc2Fine]);
		for (auto &voice : voices) voice.SetOsc2PitchFactor(osc2PitchFactor);
		break;
	}
	case kOsc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(parameters[kOsc2Split]);
		break;
	case kFilterMode:
		if (parameters[kFilterMode] != 0)
			for (auto &voice : voices) voice.SetFilterMode((FilterModes)((int)parameters[kFilterMode] - 1));
	case kVoiceMode:
		for (int i = 1; i < std::size(voices); i++) voices[i].Release();
		break;
	}

	GrayOutControls();
}
