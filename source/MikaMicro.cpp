#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(Osc1Wave)->InitEnum("Oscillator 1 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam(Osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(Osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(Osc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	GetParam(Osc2Wave)->InitEnum("Oscillator 2 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam(Osc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam(Osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(Osc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	GetParam(OscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam(FmMode)->InitEnum("FM mode", 0, 3);
	GetParam(FmCoarse)->InitInt("FM coarse", 0, 0, 48);
	GetParam(FmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam(FilterMode)->InitEnum("Filter mode", (int)FilterModes::Off, (int)FilterModes::NumFilterModes);
	GetParam(FilterMode)->SetDisplayText((int)FilterModes::Off, "Off");
	GetParam(FilterMode)->SetDisplayText((int)FilterModes::TwoPole, "Two pole");
	GetParam(FilterMode)->SetDisplayText((int)FilterModes::StateVariable, "State variable");
	GetParam(FilterMode)->SetDisplayText((int)FilterModes::FourPole, "Four pole");
	GetParam(FilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam(FilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam(FilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(VolEnvA)->InitDouble("Volume envelope attack", 0.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(VolEnvD)->InitDouble("Volume envelope decay", 1.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(VolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(VolEnvR)->InitDouble("Volume envelope release", 0.85, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(VolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(ModEnvA)->InitDouble("Modulation envelope attack", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(ModEnvD)->InitDouble("Modulation envelope decay", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(ModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(ModEnvR)->InitDouble("Modulation envelope release", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam(ModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(LfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(LfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(LfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(VolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(ModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(LfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(VolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(ModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(LfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	// master
	GetParam(VoiceMode)->InitEnum("Voice mode", (int)(VoiceModes::Legato), (int)(VoiceModes::NumVoiceModes));
	GetParam(GlideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(MasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);

	// initialize smoothed parameters
	parameters[OscMix] = GetParam(OscMix)->GetMin() + GetParam(OscMix)->GetMax() - GetParam(OscMix)->Value();
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
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, (int)Waveforms::NumWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);
	auto fmModeSwitch = pGraphics->LoadIBitmap(FMMODESWITCH_ID, FMMODESWITCH_FN, 3);

	// oscillators
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, Osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, Osc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, Osc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, Osc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, Osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, Osc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, Osc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, Osc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, OscMix, &slider));

	// fm
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, FmMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, FmCoarse, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, FmFine, &knobMiddle));

	// filter
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 22 * 4, 62 * 4, FilterMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, FilterCutoff, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, FilterResonance, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, FilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, VolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, VolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, VolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, VolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, VolEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, ModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, ModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, ModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, ModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, ModEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, LfoAmount, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, LfoFrequency, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, LfoDelay, &knobLeft));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, VolEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, ModEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, LfoFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, VolEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, ModEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, LfoCutoff, &knobMiddle));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, VoiceMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, GlideSpeed, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, MasterVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(NumParameters, 128, instanceInfo)
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

		auto voiceMode = (VoiceModes)(int)parameters[VoiceMode];
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
			case VoiceModes::Poly:
				for (auto &voice : voices)
					if (voice.GetNote() == note) voice.Release();
				break;
			case VoiceModes::Mono:
			case VoiceModes::Legato:
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
			case VoiceModes::Poly:
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
			case VoiceModes::Mono:
				voices[0].SetNote(note);
				voices[0].SetVelocity(velocity);
				voices[0].Start();
				break;
			case VoiceModes::Legato:
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
	parameters[OscMix] += (oscMix - parameters[OscMix]) * 100.0 * dt;
	parameters[FilterCutoff] += (GetParam(FilterCutoff)->Value() - parameters[FilterCutoff]) * 100.0 * dt;
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		SmoothParameters();
		auto lfoValue = lfo.Next(dt, parameters[LfoFrequency]);
		auto driftValue = GetDriftValue();
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Next(dt, lfoValue, driftValue);
		out *= parameters[MasterVolume];
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
	auto osc1Enabled = GetParam(OscMix)->Value() > 0.0;
	auto osc2Enabled = GetParam(OscMix)->Value() < 1.0;
	auto osc1Noise = (Waveforms)(int)GetParam(Osc1Wave)->Value() == Waveforms::Noise;
	auto osc2Noise = (Waveforms)(int)GetParam(Osc2Wave)->Value() == Waveforms::Noise;
	auto fmEnabled = (GetParam(FmMode)->Value() == 1 && osc1Enabled && !osc1Noise) ||
		(GetParam(FmMode)->Value() == 2 && osc2Enabled && !osc2Noise);
	auto filterEnabled = GetParam(FilterMode)->Value();
	auto modEnvEnabled = GetParam(ModEnvFm)->Value() != 0.0 || GetParam(ModEnvCutoff)->Value() != 0.0;
	auto vibratoEnabled = GetParam(LfoFm)->Value() != 0.0 || GetParam(LfoCutoff)->Value() != 0.0 ||
		GetParam(LfoAmount)->Value() < 0.0 || (GetParam(LfoAmount)->Value() > 0.0 && osc2Enabled);

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
	pGraphics->GetControl(48)->GrayOut(!GetParam(VoiceMode)->Value());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	// custom curves
	case VolEnvA:
	case VolEnvD:
	case VolEnvR:
	case ModEnvA:
	case ModEnvD:
	case ModEnvR:
		parameters[paramIdx] = 1000 - 999.9 * cosCurve(GetParam(paramIdx)->Value());
		break;
	// reversed parameters
	case LfoDelay:
	case GlideSpeed:
		parameters[paramIdx] = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	// curved parameters
	case LfoCutoff:
	{
		auto v = GetParam(paramIdx)->Value();
		parameters[paramIdx] = copysign((v * .000125) * (v * .000125) * 8000.0, v);
		break;
	}
	// smoothed parameters
	case OscMix:
		oscMix = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	case FilterCutoff:
		break;
	// normal parameters
	default:
		parameters[paramIdx] = GetParam(paramIdx)->Value();
		break;
	}

	switch (paramIdx)
	{
	case Osc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((Waveforms)(int)parameters[Osc1Wave]);
		break;
	case Osc1Coarse:
	case Osc1Fine:
	{
		auto osc1PitchFactor = pitchFactor(parameters[Osc1Coarse] + parameters[Osc1Fine]);
		for (auto &voice : voices) voice.SetOsc1PitchFactor(osc1PitchFactor);
		break;
	}
	case Osc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(parameters[Osc1Split]);
		break;
	case Osc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((Waveforms)(int)parameters[Osc2Wave]);
		break;
	case Osc2Coarse:
	case Osc2Fine:
	{
		auto osc2PitchFactor = pitchFactor(parameters[Osc2Coarse] + parameters[Osc2Fine]);
		for (auto &voice : voices) voice.SetOsc2PitchFactor(osc2PitchFactor);
		break;
	}
	case Osc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(parameters[Osc2Split]);
		break;
	case FilterMode:
		for (auto &voice : voices) voice.SetFilterMode((FilterModes)(int)parameters[FilterMode]);
		break;
	case VoiceMode:
		for (int i = 1; i < std::size(voices); i++) voices[i].Release();
		break;
	}

	GrayOutControls();
}
