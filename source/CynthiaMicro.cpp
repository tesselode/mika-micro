#include "CynthiaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 128;

enum Parameters
{
	oscillator1Wave,
	oscillator1Split,
	oscillator1Coarse,
	oscillator2Wave,
	oscillator2Split,
	oscillator2Coarse,
	oscillatorMix,
	fmCoarse,
	fmFine,
	fmEnvelopeAmount,
	filterCutoff,
	filterSmoothing,
	filterKeyTracking,
	filterEnvelope,
	envelopeAttack,
	envelopeDecay,
	envelopeSustain,
	envelopeRelease,
	envelopeVelocityAmount,
	lfoAmount,
	lfoFrequency,
	lfoDelay,
	monoMode,
	glideSpeed,
	volumeEnvelopeAmount,
	gain,
	numParameters
};

CynthiaMicro::CynthiaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, kNumPrograms, instanceInfo)
{
	TRACE;

	srand(time(NULL));

	GetParam(oscillator1Wave)->InitEnum("Oscillator 1 waveform", Saw, numWaveforms);
	GetParam(oscillator1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(oscillator1Split)->InitDouble("Oscillator 1 split", 1.0, 1.0, 1.025, .01);
	GetParam(oscillator2Wave)->InitEnum("Oscillator 2 waveform", Saw, numWaveforms);
	GetParam(oscillator2Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(oscillator2Split)->InitDouble("Oscillator 2 split", 1.0, 1.0, 1.025, .01);
	GetParam(oscillatorMix)->InitDouble("Oscillator mix", 0.0, 0.0, 1.0, .01);

	GetParam(fmCoarse)->InitInt("FM coarse", 0, 0, 24, "semitones");
	GetParam(fmFine)->InitDouble("FM fine", 0, -1.0, 1.0, .01, "semitones");
	GetParam(fmEnvelopeAmount)->InitDouble("FM envelope amount", 0.0, -24.0, 24.0, .01, "semitones");

	GetParam(filterCutoff)->InitDouble("Filter cutoff", 20000.0, 20., 20000., .01, "hz", "", 1.0);
	GetParam(filterSmoothing)->InitDouble("Filter smoothing", 0.0, 0.0, .75, .01);
	GetParam(filterKeyTracking)->InitDouble("Filter key tracking", 0.0, 0.0, 1.0, .01);
	GetParam(filterEnvelope)->InitDouble("Filter envelope amount", 0.0, -20000.0, 20000.0, .01);

	GetParam(envelopeAttack)->InitDouble("Envelope attack", 0.1, 0.1, 100., .01, "", "", .05);
	GetParam(envelopeDecay)->InitDouble("Envelope decay", 99., 0.1, 100., .01, "", "", .05);
	GetParam(envelopeSustain)->InitDouble("Envelope sustain", .5, 0., 1., .01);
	GetParam(envelopeRelease)->InitDouble("Envelope release", 0.1, 0.1, 100., .01, "", "", .05);
	GetParam(envelopeVelocityAmount)->InitDouble("Envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);

	GetParam(lfoAmount)->InitDouble("Vibrato amount", 0, -0.1, 0.1, .01);
	GetParam(lfoFrequency)->InitDouble("Vibrato speed", 5.0, 1.0, 10.0, .01, "hz");
	GetParam(lfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 100., .01, "", "", .01);

	GetParam(monoMode)->InitBool("Mono mode", true);
	GetParam(glideSpeed)->InitDouble("Glide speed", 10, 10, 1000., .01, "", "", .05);
	GetParam(volumeEnvelopeAmount)->InitDouble("Volume envelope amount", 0.0, 0.0, 1.0, .01);
	GetParam(gain)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);

	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 54);
	IBitmap toggle = pGraphics->LoadIBitmap(SWITCH_ID, SWITCH_FN, 2);
	IBitmap waveformSwitch = pGraphics->LoadIBitmap(WAVEFORM_SWITCH_ID, WAVEFORM_SWITCH_FN, 5);

	pGraphics->AttachControl(new ISwitchControl(this, 32 * 4, 5 * 4, oscillator1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 5 * 4, oscillator1Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 5 * 4, oscillator1Split, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 88 * 4, 5 * 4, oscillatorMix, &knob));
	pGraphics->AttachControl(new ISwitchControl(this, 112 * 4, 5 * 4, oscillator2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 5 * 4, oscillator2Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 5 * 4, oscillator2Split, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 32 * 4, 37 * 4, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 37 * 4, fmFine, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 37 * 4, fmEnvelopeAmount, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 96 * 4, 37 * 4, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 112 * 4, 37 * 4, filterSmoothing, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 37 * 4, filterKeyTracking, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 37 * 4, filterEnvelope, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 8 * 4, envelopeAttack, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 24 * 4, envelopeDecay, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 40 * 4, envelopeSustain, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 56 * 4, envelopeRelease, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 72 * 4, envelopeVelocityAmount, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 32 * 4, 69 * 4, lfoAmount, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 69 * 4, lfoFrequency, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 69 * 4, lfoDelay, &knob));

	pGraphics->AttachControl(new ISwitchControl(this, 96 * 4, 69 * 4, monoMode, &toggle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 112 * 4, 69 * 4, glideSpeed, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 69 * 4, volumeEnvelopeAmount, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 69 * 4, gain, &knob));

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", kNumPrograms);
}

CynthiaMicro::~CynthiaMicro() {}

int CynthiaMicro::GetQuietestVoice(bool releasedOnly)
{
	double quietestVolume = 100;
	int quietestVoice = -1;
	for (int i = 0; i < numVoices; i++)
	{
		if (voices[i].IsReleased() || !releasedOnly)
		{
			if (voices[i].GetVolume() < quietestVolume)
			{
				quietestVolume = voices[i].GetVolume();
				quietestVoice = i;
			}
		}
	}
	return quietestVoice;
}

int CynthiaMicro::PickVoice()
{
	int v = GetQuietestVoice(true);
	if (v != -1) return v;
	return GetQuietestVoice(false);
}

void CynthiaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		lfo.Update();
		double lfoValue = lfo.Get(Sine);

		while (!midiQueue.Empty())
		{
			IMidiMsg* message = midiQueue.Peek();
			if (message->mOffset > s) break;

			if (message->StatusMsg() == IMidiMsg::kNoteOn)
			{
				if (mono)
				{
					voices[0].SetNote(message->NoteNumber());
					voices[0].SetVelocity(message->Velocity());
					if (heldNotes.empty())
					{
						voices[0].Start();
					}
				}
				else
				{
					int v = PickVoice();
					voices[v].SetNote(message->NoteNumber());
					voices[v].SetVelocity(message->Velocity());
					voices[v].Start();
				}
				heldNotes.push_back(message->NoteNumber());
				velocities.push_back(message->Velocity());
			}
			else if (message->StatusMsg() == IMidiMsg::kNoteOff)
			{
				for (int i = 0; i < heldNotes.size(); i++)
				{
					if (heldNotes[i] == message->NoteNumber())
					{
						heldNotes.erase(heldNotes.begin() + i);
						velocities.erase(velocities.begin() + i);
						break;
					}
				}
				if (mono)
				{
					if (heldNotes.empty())
					{
						voices[0].Release();
					}
					else
					{
						voices[0].SetNote(heldNotes.back());
						voices[0].SetVelocity(velocities.back());
					}
				}
				else
				{
					for (int i = 0; i < numVoices; i++)
					{
						if (voices[i].GetNote() == message->NoteNumber())
						{
							voices[i].Release();
						}
					}
				}
			}
			else if (message->StatusMsg() == IMidiMsg::kPitchWheel)
			{
				for (int i = 0; i < numVoices; i++)
				{
					voices[i].SetPitchBend(message->PitchWheel());
				}
			}

			midiQueue.Remove();
		}

		double out = 0.0;
		for (int i = 0; i < numVoices; i++)
		{
			out += .5 * voices[i].Next(lfoValue);
		}
		out *= masterVolume;
		*out1 = out;
		*out2 = out;
	}
}

void CynthiaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	IMidiMsg::EStatusMsg status = message->StatusMsg();
	if (status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff || status == IMidiMsg::kPitchWheel)
	{
		midiQueue.Add(message);
	}
}

void CynthiaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);

	lfo.SetSampleRate(GetSampleRate());
	for (int i = 0; i < numVoices; i++)
	{
		voices[i].SetSampleRate(GetSampleRate());
	}
}

void CynthiaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	IParam* param = GetParam(paramIdx);
	double value = param->Value();
	double reverseValue = param->GetMin() + param->GetMax() - value;

	if (paramIdx == lfoFrequency)
	{
		lfo.SetFrequency(value);
	}
	else if (paramIdx == monoMode)
	{
		mono = value;
		for (int i = 0; i < numVoices; i++)
		{
			voices[i].SetMono(value);
		}
		for (int i = 1; i < numVoices; i++)
		{
			voices[i].Release();
		}
	}
	else if (paramIdx == gain)
	{
		masterVolume = value;
	}
	else
	{
		for (int i = 0; i < numVoices; i++)
		{
			if (paramIdx == oscillator1Wave) voices[i].SetOscillator1Wave((Waveform)(int)value);
			if (paramIdx == oscillator1Split) voices[i].SetOscillator1Split(value);
			if (paramIdx == oscillator1Coarse) voices[i].SetOscillator1Coarse(value);
			if (paramIdx == oscillator2Wave) voices[i].SetOscillator2Wave((Waveform)(int)value);
			if (paramIdx == oscillator2Split) voices[i].SetOscillator2Split(value);
			if (paramIdx == oscillator2Coarse) voices[i].SetOscillator2Coarse(value);
			if (paramIdx == oscillatorMix) voices[i].SetOscillatorMix(value);

			if (paramIdx == fmCoarse) voices[i].SetFmCoarse(value);
			if (paramIdx == fmFine) voices[i].SetFmFine(value);
			if (paramIdx == fmEnvelopeAmount) voices[i].SetFmEnvelopeAmount(value);

			if (paramIdx == filterCutoff) voices[i].SetFilterCutoff(value);
			if (paramIdx == filterSmoothing) voices[i].SetFilterResonance(value);
			if (paramIdx == filterKeyTracking) voices[i].SetFilterKeyTracking(value);
			if (paramIdx == filterEnvelope) voices[i].SetFilterEnvelopeAmount(value);

			if (paramIdx == envelopeAttack) voices[i].SetEnvelopeAttack(reverseValue);
			if (paramIdx == envelopeDecay) voices[i].SetEnvelopeDecay(reverseValue);
			if (paramIdx == envelopeSustain) voices[i].SetEnvelopeSustain(value);
			if (paramIdx == envelopeRelease) voices[i].SetEnvelopeRelease(reverseValue);
			if (paramIdx == envelopeVelocityAmount) voices[i].SetEnvelopeVelocityAmount(value);

			if (paramIdx == lfoAmount) voices[i].SetLfoAmount(value);
			if (paramIdx == lfoDelay) voices[i].SetLfoDelay(reverseValue);

			if (paramIdx == glideSpeed) voices[i].SetGlideSpeed(reverseValue);
			if (paramIdx == volumeEnvelopeAmount) voices[i].SetVolumeEnvelopeAmount(value);
		}
	}

	// gray out controls
	pGraphics->GetControl(oscillator1Coarse + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 1.0 && GetParam(fmCoarse)->Value() == 0 && GetParam(fmFine)->Value() == 0 && GetParam(fmEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(oscillator1Split + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 1.0 && GetParam(fmCoarse)->Value() == 0 && GetParam(fmFine)->Value() == 0 && GetParam(fmEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(oscillator1Wave + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 1.0);
	pGraphics->GetControl(oscillator2Coarse + 2)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);
	pGraphics->GetControl(oscillator2Split + 2)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);
	pGraphics->GetControl(oscillator2Wave + 2)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);

	pGraphics->GetControl(fmCoarse + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);
	pGraphics->GetControl(fmFine + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);
	pGraphics->GetControl(fmEnvelopeAmount + 1)->GrayOut(GetParam(oscillatorMix)->Value() == 0.0);

	pGraphics->GetControl(filterSmoothing + 1)->GrayOut(GetParam(filterCutoff)->Value() == 20000.0);
	pGraphics->GetControl(filterKeyTracking + 1)->GrayOut(GetParam(filterCutoff)->Value() == 20000.0);
	pGraphics->GetControl(filterEnvelope + 1)->GrayOut(GetParam(filterCutoff)->Value() == 20000.0);

	pGraphics->GetControl(lfoFrequency + 1)->GrayOut(GetParam(lfoAmount)->Value() == 0 || (GetParam(lfoAmount)->Value() < 0 && GetParam(oscillatorMix)->Value() == 0.0));
	pGraphics->GetControl(lfoDelay + 1)->GrayOut(GetParam(lfoAmount)->Value() == 0 || (GetParam(lfoAmount)->Value() < 0 && GetParam(oscillatorMix)->Value() == 0.0));

	pGraphics->GetControl(glideSpeed + 1)->GrayOut(!GetParam(monoMode)->Value());

	pGraphics->GetControl(envelopeAttack + 1)->GrayOut(GetParam(fmEnvelopeAmount)->Value() == 0 && GetParam(filterEnvelope)->Value() == 0 && GetParam(volumeEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(envelopeDecay + 1)->GrayOut(GetParam(fmEnvelopeAmount)->Value() == 0 && GetParam(filterEnvelope)->Value() == 0 && GetParam(volumeEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(envelopeSustain + 1)->GrayOut(GetParam(fmEnvelopeAmount)->Value() == 0 && GetParam(filterEnvelope)->Value() == 0 && GetParam(volumeEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(envelopeRelease + 1)->GrayOut(GetParam(fmEnvelopeAmount)->Value() == 0 && GetParam(filterEnvelope)->Value() == 0 && GetParam(volumeEnvelopeAmount)->Value() == 0);
	pGraphics->GetControl(envelopeVelocityAmount + 1)->GrayOut(GetParam(fmEnvelopeAmount)->Value() == 0 && GetParam(filterEnvelope)->Value() == 0 && GetParam(volumeEnvelopeAmount)->Value() == 0);
}
