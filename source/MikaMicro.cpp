#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	P(kOsc1Wave)->InitEnum("Oscillator 1 waveform", kWaveformSaw, kNumWaveforms);
	P(kOsc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	P(kOsc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	P(kOsc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	P(kOsc2Wave)->InitEnum("Oscillator 2 waveform", kWaveformSaw, kNumWaveforms);
	P(kOsc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	P(kOsc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	P(kOsc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	P(kOscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	P(kFmMode)->InitEnum("FM mode", 0, 3);
	P(kFmCoarse)->InitInt("FM coarse", 0, 0, 48);
	P(kFmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	P(kFilterEnabled)->InitBool("Filter enabled", false);
	P(kFilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	P(kFilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	P(kFilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	P(kVolEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	P(kVolEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	P(kVolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	P(kVolEnvR)->InitDouble("Volume envelope release", 925.0, 0.5, 1000.0, .01, "", "", .025);
	P(kVolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	P(kModEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.5, 1000.0, .01, "", "", .025);
	P(kModEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	P(kModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	P(kModEnvR)->InitDouble("Modulation envelope release", 998.0, 0.5, 1000.0, .01, "", "", .025);
	P(kModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	P(kLfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	P(kLfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	P(kLfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	P(kVolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	P(kModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	P(kLfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	P(kVolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	P(kModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	P(kLfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	// master
	P(kVoiceMode)->InitEnum("Voice mode", kVoiceModeLegato, kNumVoiceModes);
	P(kGlideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	P(kMasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);
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
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 62 * 4, kFilterEnabled, &toggleSwitch));
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
  :	IPLUG_CTOR(kNumParameters, 128, instanceInfo),
	gen(rd()),
	dist(-1.0, 1.0)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

MikaMicro::~MikaMicro() { }

void MikaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	midiQueue.Add(message);
}

void MikaMicro::StopNote(EVoiceModes voiceMode, int note)
{
	heldNotes.erase(
		std::remove(
			std::begin(heldNotes),
			std::end(heldNotes),
			note),
		std::end(heldNotes));
	switch (voiceMode)
	{
	case kVoiceModePoly:
		for (auto& voice : voices)
			if (voice.GetNote() == note) voice.Release();
		break;
	case kVoiceModeMono:
	case kVoiceModeLegato:
		if (heldNotes.empty())
			voices[0].Release();
		else
			voices[0].SetNote(heldNotes.back());
		break;
	}
}

void MikaMicro::StartNote(EVoiceModes voiceMode, int note, double velocity)
{
	switch (voiceMode)
	{
	case kVoiceModePoly:
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
		voice->SetVelocity(velocity);
		voice->ResetPitch();
		voice->Start();
		break;
	}
	case kVoiceModeMono:
		voices[0].SetNote(note);
		voices[0].SetVelocity(velocity);
		voices[0].Start();
		break;
	case kVoiceModeLegato:
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
}

void MikaMicro::FlushMidiQueue(int s)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > s) break;

		auto voiceMode = (EVoiceModes)(int)V(kVoiceMode);
		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = pow(message->Velocity() * .0078125, 1.25);
		bool noteOff = status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && velocity == 0);

		if (noteOff)
			StopNote(voiceMode, note);
		else if (status == IMidiMsg::kNoteOn)
			StartNote(voiceMode, note, velocity);
		else if (status == IMidiMsg::kPitchWheel)
		{
			auto f = pitchFactor(message->PitchWheel() * 2);
			for (auto& voice : voices) voice.SetPitchBendFactor(f);
		}
		else if (status == IMidiMsg::kAllNotesOff)
			for (auto& voice : voices) voice.Release();

		midiQueue.Remove();
	}
}

double MikaMicro::GetDriftValue()
{
	driftVelocity += dist(gen) * 10000 * dt;
	driftVelocity -= driftVelocity * 2 * dt;
	driftPhase += driftVelocity * dt;
	return .001 * sin(driftPhase);
}

double MikaMicro::GetVoices()
{
	auto lfoValue = lfo.Next(V(kLfoFrequency), kWaveformSine);
	auto driftValue = GetDriftValue();
	auto out = 0.0;
	for (auto &voice : voices) out += voice.Next(lfoValue, driftValue);
	return out * V(kMasterVolume);
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		FlushMidiQueue(s);
		auto out = GetVoices();
		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
	lfo.SetSampleRate(GetSampleRate());
	for (auto &voice : voices) voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::GrayOutControls()
{
	auto osc1Enabled = V(kOscMix) > 0.0;
	auto osc2Enabled = V(kOscMix) < 1.0;
	auto osc1Noise = (EWaveforms)(int)V(kOsc1Wave) == kWaveformNoise;
	auto osc2Noise = (EWaveforms)(int)V(kOsc2Wave) == kWaveformNoise;
	auto fmEnabled = (V(kFmMode) == 1 && osc1Enabled && !osc1Noise) ||
		(V(kFmMode) == 2 && osc2Enabled && !osc2Noise);
	auto filterEnabled = V(kFilterEnabled);
	auto modEnvEnabled = V(kModEnvFm) != 0.0 || V(kModEnvCutoff) != 0.0;
	auto vibratoEnabled = V(kLfoFm) != 0.0 || V(kLfoCutoff) != 0.0 ||
		V(kLfoAmount) < 0.0 || (V(kLfoAmount) > 0.0 && osc2Enabled);

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
	pGraphics->GetControl(48)->GrayOut(!V(kVoiceMode));
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	auto value = V(paramIdx);

	// reverse parameters
	switch (paramIdx)
	{
	case kOscMix:
	case kVolEnvA:
	case kVolEnvD:
	case kVolEnvR:
	case kModEnvA:
	case kModEnvD:
	case kModEnvR:
	case kLfoDelay:
	case kGlideSpeed:
		value = P(paramIdx)->GetMax() + P(paramIdx)->GetMin() - value;
	}

	switch (paramIdx)
	{
	case kOsc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((EWaveforms)(int)value);
		break;
	case kOsc1Coarse:
	case kOsc1Fine:
		for (auto &voice : voices)
			voice.SetOsc1Tune(V(kOsc1Coarse) + V(kOsc1Fine));
		break;
	case kOsc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(value);
		break;
	case kOsc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((EWaveforms)(int)value);
		break;
	case kOsc2Coarse:
	case kOsc2Fine:
		for (auto &voice : voices)
			voice.SetOsc2Tune(V(kOsc2Coarse) + V(kOsc2Fine));
		break;
	case kOsc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(value);
		break;
	case kOscMix:
		for (auto &voice : voices) voice.SetOscMix(value);
		break;
	case kFmMode:
		for (auto &voice : voices) voice.SetFmMode(value);
		break;
	case kFmCoarse:
		for (auto &voice : voices) voice.SetFmCoarse(value);
		break;
	case kFmFine:
		for (auto &voice : voices) voice.SetFmFine(value);
		break;
	case kFilterEnabled:
		for (auto &voice : voices) voice.SetFilterEnabled(value);
		break;
	case kFilterCutoff:
		for (auto &voice : voices) voice.SetFilterCutoff(value);
		break;
	case kFilterResonance:
		for (auto &voice : voices) voice.SetFilterResonance(value);
		break;
	case kFilterKeyTrack:
		for (auto &voice : voices) voice.SetFilterKeyTrack(value);
		break;
	case kVolEnvA:
		for (auto &voice : voices) voice.SetVolumeEnvelopeAttack(value);
		break;
	case kVolEnvD:
		for (auto &voice : voices) voice.SetVolumeEnvelopeDecay(value);
		break;
	case kVolEnvS:
		for (auto &voice : voices) voice.SetVolumeEnvelopeSustain(value);
		break;
	case kVolEnvR:
		for (auto &voice : voices) voice.SetVolumeEnvelopeRelease(value);
		break;
	case kVolEnvV:
		for (auto &voice : voices) voice.SetVolumeEnvelopeVelocitySensitivity(value);
		break;
	case kModEnvA:
		for (auto &voice : voices) voice.SetModEnvelopeAttack(value);
		break;
	case kModEnvD:
		for (auto &voice : voices) voice.SetModEnvelopeDecay(value);
		break;
	case kModEnvS:
		for (auto &voice : voices) voice.SetModEnvelopeSustain(value);
		break;
	case kModEnvR:
		for (auto &voice : voices) voice.SetModEnvelopeRelease(value);
		break;
	case kModEnvV:
		for (auto &voice : voices) voice.SetModEnvelopeVelocitySensitivity(value);
		break;
	case kLfoAmount:
		for (auto &voice : voices) voice.SetLfoAmount(value);
		break;
	case kLfoDelay:
		for (auto &voice : voices) voice.SetLfoDelay(value);
		break;
	case kVolEnvFm:
		for (auto &voice : voices) voice.SetVolumeEnvelopeFm(value);
		break;
	case kModEnvFm:
		for (auto &voice : voices) voice.SetModEnvelopeFm(value);
		break;
	case kLfoFm:
		for (auto &voice : voices) voice.SetLfoFm(value);
		break;
	case kVolEnvCutoff:
		for (auto &voice : voices) voice.SetVolumeEnvelopeCutoff(value);
		break;
	case kModEnvCutoff:
		for (auto &voice : voices) voice.SetModEnvelopeCutoff(value);
		break;
	case kLfoCutoff:
		for (auto &voice : voices) voice.SetLfoCutoff(value);
		break;
	case kVoiceMode:
		for (int i = 1; i < voices.size(); i++) voices[i].Release();
		break;
	case kGlideSpeed:
		for (auto &voice : voices) voice.SetGlideSpeed(value);
		break;
	}

	GrayOutControls();
}