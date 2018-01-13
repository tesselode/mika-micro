#include "CynthiaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum Parameters
{
	oscillator1Split,
	oscillator1Coarse,
	oscillator2Split,
	oscillator2Coarse,
	oscillatorMix,
	fmCoarse,
	fmFine,
	filterCutoff,
	filterResonance,
	filterKeyTracking,
	envelopeAttack,
	envelopeDecay,
	envelopeSustain,
	envelopeRelease,
	lfoAmount,
	lfoFrequency,
	monoMode,
	glideSpeed,
	numParameters
};

CynthiaMicro::CynthiaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, kNumPrograms, instanceInfo)
{
	TRACE;

	GetParam(oscillator1Split)->InitDouble("Oscillator 1 split", 1.0, 1.0, 1.1, .01);
	GetParam(oscillator1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(oscillator2Split)->InitDouble("Oscillator 2 split", 1.0, 1.0, 1.1, .01);
	GetParam(oscillator2Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(oscillatorMix)->InitDouble("Oscillator mix", 0.0, 0.0, 1.0, .01);

	GetParam(fmCoarse)->InitInt("FM coarse", 0, 0, 24, "semitones");
	GetParam(fmFine)->InitDouble("FM fine", 0, -1.0, 1.0, .01, "semitones");

	GetParam(filterCutoff)->InitDouble("Filter cutoff", 20000.0, 20., 20000., .01, "hz");
	GetParam(filterResonance)->InitDouble("Filter resonance", 0.0, 0.0, .75, .01);
	GetParam(filterKeyTracking)->InitDouble("Filter key tracking", 0.0, 0.0, 1.0, .01);

	GetParam(envelopeAttack)->InitDouble("Envelope attack", 100., 0.1, 100., .01);
	GetParam(envelopeDecay)->InitDouble("Envelope decay", 1., 0.1, 100., .01);
	GetParam(envelopeSustain)->InitDouble("Envelope sustain", .5, 0., 1., .01);
	GetParam(envelopeRelease)->InitDouble("Envelope release", 1., 0.1, 100., .01);

	GetParam(lfoAmount)->InitDouble("Vibrato amount", 0, -0.1, 0.1, .01);
	GetParam(lfoFrequency)->InitDouble("Vibrato speed", 5.0, 1.0, 10.0, .01, "hz");

	GetParam(monoMode)->InitBool("Mono mode", true);
	GetParam(glideSpeed)->InitDouble("Glide speed", 1000., 0.1, 1000., .01);

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 54);
	IBitmap toggle = pGraphics->LoadIBitmap(SWITCH_ID, SWITCH_FN, 2);
	IBitmap waveformSwitch = pGraphics->LoadIBitmap(WAVEFORM_SWITCH_ID, WAVEFORM_SWITCH_FN, 5);

	//pGraphics->AttachControl(new ISwitchControl(this, 32 * 4, 5 * 4, osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 5 * 4, oscillator1Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 5 * 4, oscillator1Split, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 88 * 4, 5 * 4, oscillatorMix, &knob));
	//pGraphics->AttachControl(new ISwitchControl(this, 112 * 4, 5 * 4, osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 5 * 4, oscillator2Coarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 5 * 4, oscillator2Split, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 32 * 4, 37 * 4, fmCoarse, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 37 * 4, fmFine, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 37 * 4, fmEnv, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 96 * 4, 37 * 4, filterCutoff, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 112 * 4, 37 * 4, filterResonance, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 37 * 4, filterKeyTracking, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 37 * 4, filterEnv, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 8 * 4, envelopeAttack, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 24 * 4, envelopeDecay, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 40 * 4, envelopeSustain, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 56 * 4, envelopeRelease, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 176 * 4, 72 * 4, modEnvVel, &knob));

	pGraphics->AttachControl(new IKnobMultiControl(this, 32 * 4, 69 * 4, lfoAmount, &knob));
	pGraphics->AttachControl(new IKnobMultiControl(this, 48 * 4, 69 * 4, lfoFrequency, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 64 * 4, 69 * 4, lfoDelay, &knob));

	pGraphics->AttachControl(new ISwitchControl(this, 96 * 4, 69 * 4, monoMode, &toggle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 112 * 4, 69 * 4, glideSpeed, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 128 * 4, 69 * 4, volumeEnv, &knob));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 144 * 4, 69 * 4, gain, &knob));

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
					if (heldNotes.empty())
					{
						voices[0].Start();
					}
				}
				else
				{
					int v = PickVoice();
					voices[v].SetNote(message->NoteNumber());
					voices[v].Start();
				}
				heldNotes.push_back(message->NoteNumber());
			}
			else if (message->StatusMsg() == IMidiMsg::kNoteOff)
			{
				for (int i = 0; i < heldNotes.size(); i++)
				{
					if (heldNotes[i] == message->NoteNumber())
					{
						heldNotes.erase(heldNotes.begin() + i);
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

			midiQueue.Remove();
		}

		double out = 0.0;
		for (int i = 0; i < numVoices; i++)
		{
			out += .25 * voices[i].Next(lfoValue);
		}
		*out1 = out;
		*out2 = out;
	}
}

void CynthiaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	if (message->StatusMsg() == IMidiMsg::kNoteOn || message->StatusMsg() == IMidiMsg::kNoteOff)
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
	}
	else
	{
		for (int i = 0; i < numVoices; i++)
		{
			if (paramIdx == oscillator1Split) voices[i].SetOscillator1Split(value);
			if (paramIdx == oscillator1Coarse) voices[i].SetOscillator1Coarse(value);
			if (paramIdx == oscillator2Split) voices[i].SetOscillator2Split(value);
			if (paramIdx == oscillator2Coarse) voices[i].SetOscillator2Coarse(value);
			if (paramIdx == oscillatorMix) voices[i].SetOscillatorMix(value);

			if (paramIdx == fmCoarse) voices[i].SetFmCoarse(value);
			if (paramIdx == fmFine) voices[i].SetFmFine(value);

			if (paramIdx == filterCutoff) voices[i].SetFilterCutoff(value);
			if (paramIdx == filterResonance) voices[i].SetFilterResonance(value);
			if (paramIdx == filterKeyTracking) voices[i].SetFilterKeyTracking(value);

			if (paramIdx == envelopeAttack) voices[i].SetEnvelopeAttack(value);
			if (paramIdx == envelopeDecay) voices[i].SetEnvelopeDecay(value);
			if (paramIdx == envelopeSustain) voices[i].SetEnvelopeSustain(value);
			if (paramIdx == envelopeRelease) voices[i].SetEnvelopeRelease(value);

			if (paramIdx == lfoAmount) voices[i].SetLfoAmount(value);

			if (paramIdx == glideSpeed) voices[i].SetGlideSpeed(value);
		}
	}
}
