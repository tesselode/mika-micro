#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam((int)Parameters::Osc1Wave)->InitEnum("Oscillator 1 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)Parameters::Osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam((int)Parameters::Osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam((int)Parameters::Osc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	GetParam((int)Parameters::Osc2Wave)->InitEnum("Oscillator 2 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)Parameters::Osc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam((int)Parameters::Osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam((int)Parameters::Osc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	GetParam((int)Parameters::OscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam((int)Parameters::FmMode)->InitEnum("FM mode", (int)FmModes::Off, (int)FmModes::NumFmModes);
	GetParam((int)Parameters::FmCoarse)->InitInt("FM coarse", 0, 0, 48);
	GetParam((int)Parameters::FmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam((int)Parameters::FilterMode)->InitEnum("Filter mode", (int)FilterModes::Off, (int)FilterModes::NumFilterModes);
	GetParam((int)Parameters::FilterMode)->SetDisplayText((int)FilterModes::Off, "Off");
	GetParam((int)Parameters::FilterMode)->SetDisplayText((int)FilterModes::TwoPole, "Two pole");
	GetParam((int)Parameters::FilterMode)->SetDisplayText((int)FilterModes::StateVariable, "State variable");
	GetParam((int)Parameters::FilterMode)->SetDisplayText((int)FilterModes::FourPole, "Four pole");
	GetParam((int)Parameters::FilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam((int)Parameters::FilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam((int)Parameters::FilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam((int)Parameters::VolEnvA)->InitDouble("Volume envelope attack", 0.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::VolEnvD)->InitDouble("Volume envelope decay", 1.0, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::VolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam((int)Parameters::VolEnvR)->InitDouble("Volume envelope release", 0.85, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::VolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam((int)Parameters::ModEnvA)->InitDouble("Modulation envelope attack", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::ModEnvD)->InitDouble("Modulation envelope decay", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::ModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam((int)Parameters::ModEnvR)->InitDouble("Modulation envelope release", 0.933, 0.0, 1.0, .01, "", "", 0.1);
	GetParam((int)Parameters::ModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam((int)Parameters::LfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam((int)Parameters::LfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam((int)Parameters::LfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam((int)Parameters::VolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)Parameters::ModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)Parameters::LfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)Parameters::VolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam((int)Parameters::ModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam((int)Parameters::LfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	// master
	GetParam((int)Parameters::VoiceMode)->InitEnum("Voice mode", (int)(VoiceModes::Legato), (int)(VoiceModes::NumVoiceModes));
	GetParam((int)Parameters::GlideLength)->InitDouble("Glide length", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam((int)Parameters::MasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);

	// initialize smoothed parameters
	parameters[(int)Parameters::OscMix] = GetParam((int)Parameters::OscMix)->GetMin() + GetParam((int)Parameters::OscMix)->GetMax() - GetParam((int)Parameters::OscMix)->Value();
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
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, (int)Parameters::Osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, (int)Parameters::Osc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, (int)Parameters::Osc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, (int)Parameters::Osc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, (int)Parameters::Osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, (int)Parameters::Osc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, (int)Parameters::Osc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, (int)Parameters::Osc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, (int)Parameters::OscMix, &slider));

	// fm
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, (int)Parameters::FmMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, (int)Parameters::FmCoarse, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, (int)Parameters::FmFine, &knobMiddle));

	// filter
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 22 * 4, 62 * 4, (int)Parameters::FilterMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, (int)Parameters::FilterCutoff, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, (int)Parameters::FilterResonance, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, (int)Parameters::FilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, (int)Parameters::LfoAmount, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, (int)Parameters::LfoFrequency, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, (int)Parameters::LfoDelay, &knobLeft));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, (int)Parameters::VolEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, (int)Parameters::ModEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, (int)Parameters::LfoFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, (int)Parameters::VolEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, (int)Parameters::ModEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, (int)Parameters::LfoCutoff, &knobMiddle));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, (int)Parameters::VoiceMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, (int)Parameters::GlideLength, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, (int)Parameters::MasterVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR((int)Parameters::NumParameters, 128, instanceInfo)
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

		auto voiceMode = (VoiceModes)(int)parameters[(int)Parameters::VoiceMode];
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
	parameters[(int)Parameters::OscMix] += (oscMix - parameters[(int)Parameters::OscMix]) * 100.0 * dt;
	parameters[(int)Parameters::FilterCutoff] += (GetParam((int)Parameters::FilterCutoff)->Value() - parameters[(int)Parameters::FilterCutoff]) * 100.0 * dt;
	parameters[(int)Parameters::MasterVolume] += (GetParam((int)Parameters::MasterVolume)->Value() - parameters[(int)Parameters::MasterVolume]) * 100.0 * dt;
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		SmoothParameters();
		auto lfoValue = lfo.Next(dt, parameters[(int)Parameters::LfoFrequency]);
		auto driftValue = GetDriftValue();
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Next(dt, lfoValue, driftValue);
		out *= parameters[(int)Parameters::MasterVolume];
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
	auto osc1Enabled = GetParam((int)Parameters::OscMix)->Value() > 0.0;
	auto osc2Enabled = GetParam((int)Parameters::OscMix)->Value() < 1.0;
	auto osc1Noise = (Waveforms)(int)GetParam((int)Parameters::Osc1Wave)->Value() == Waveforms::Noise;
	auto osc2Noise = (Waveforms)(int)GetParam((int)Parameters::Osc2Wave)->Value() == Waveforms::Noise;
	auto fmEnabled = (GetParam((int)Parameters::FmMode)->Value() == 1 && osc1Enabled && !osc1Noise) ||
		(GetParam((int)Parameters::FmMode)->Value() == 2 && osc2Enabled && !osc2Noise);
	auto filterEnabled = GetParam((int)Parameters::FilterMode)->Value();
	auto modEnvEnabled = GetParam((int)Parameters::ModEnvFm)->Value() != 0.0 || GetParam((int)Parameters::ModEnvCutoff)->Value() != 0.0;
	auto vibratoEnabled = GetParam((int)Parameters::LfoFm)->Value() != 0.0 || GetParam((int)Parameters::LfoCutoff)->Value() != 0.0 ||
		GetParam((int)Parameters::LfoAmount)->Value() < 0.0 || (GetParam((int)Parameters::LfoAmount)->Value() > 0.0 && osc2Enabled);

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
	pGraphics->GetControl(48)->GrayOut(!GetParam((int)Parameters::VoiceMode)->Value());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch ((Parameters)paramIdx)
	{
	// custom curves
	case Parameters::VolEnvA:
	case Parameters::VolEnvD:
	case Parameters::VolEnvR:
	case Parameters::ModEnvA:
	case Parameters::ModEnvD:
	case Parameters::ModEnvR:
		parameters[paramIdx] = 1000 - 999.9 * cosCurve(GetParam(paramIdx)->Value());
		break;
	case Parameters::LfoCutoff:
	{
		auto v = GetParam(paramIdx)->Value();
		parameters[paramIdx] = copysign((v * .000125) * (v * .000125) * 8000.0, v);
		break;
	}
	// reversed parameters
	case Parameters::LfoDelay:
	case Parameters::GlideLength:
		parameters[paramIdx] = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	// smoothed parameters
	case Parameters::OscMix:
		oscMix = GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value();
		break;
	case Parameters::FilterCutoff:
	case Parameters::MasterVolume:
		break;
	// normal parameters
	default:
		parameters[paramIdx] = GetParam(paramIdx)->Value();
		break;
	}

	switch ((Parameters)paramIdx)
	{
	case Parameters::Osc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((Waveforms)(int)parameters[(int)Parameters::Osc1Wave]);
		break;
	case Parameters::Osc1Coarse:
	case Parameters::Osc1Fine:
	{
		auto osc1PitchFactor = pitchFactor(parameters[(int)Parameters::Osc1Coarse] + parameters[(int)Parameters::Osc1Fine]);
		for (auto &voice : voices) voice.SetOsc1PitchFactor(osc1PitchFactor);
		break;
	}
	case Parameters::Osc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(parameters[(int)Parameters::Osc1Split]);
		break;
	case Parameters::Osc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((Waveforms)(int)parameters[(int)Parameters::Osc2Wave]);
		break;
	case Parameters::Osc2Coarse:
	case Parameters::Osc2Fine:
	{
		auto osc2PitchFactor = pitchFactor(parameters[(int)Parameters::Osc2Coarse] + parameters[(int)Parameters::Osc2Fine]);
		for (auto &voice : voices) voice.SetOsc2PitchFactor(osc2PitchFactor);
		break;
	}
	case Parameters::Osc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(parameters[(int)Parameters::Osc2Split]);
		break;
	case Parameters::FilterMode:
		for (auto &voice : voices) voice.SetFilterMode((FilterModes)(int)parameters[(int)Parameters::FilterMode]);
		break;
	case Parameters::VoiceMode:
		for (int i = 1; i < std::size(voices); i++) voices[i].Release();
		break;
	}

	GrayOutControls();
}
