#include "CynthiaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum Parameters
{
	oscillator1Split,
	oscillatorMix,
	envelopeAttack,
	envelopeDecay,
	envelopeSustain,
	envelopeRelease,
	numParameters
};

CynthiaMicro::CynthiaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, kNumPrograms, instanceInfo)
{
	TRACE;

	GetParam(oscillator1Split)->InitDouble("Oscillator 1 split", 1.1, 1.0, 1.1, .01);
	GetParam(oscillatorMix)->InitDouble("Oscillator mix", 0.0, 0.0, 1.0, .01);

	GetParam(envelopeAttack)->InitDouble("Envelope attack", 100., 0.1, 100., .01);
	GetParam(envelopeDecay)->InitDouble("Envelope decay", 1., 0.1, 100., .01);
	GetParam(envelopeSustain)->InitDouble("Envelope sustain", .5, 0., 1., .01);
	GetParam(envelopeRelease)->InitDouble("Envelope release", 1., 0.1, 100., .01);

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

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
		while (!midiQueue.Empty())
		{
			IMidiMsg* message = midiQueue.Peek();
			if (message->mOffset > s) break;

			if (message->StatusMsg() == IMidiMsg::kNoteOn)
			{
				int v = PickVoice();
				voices[v].SetNote(message->NoteNumber());
				voices[v].Start();
			}
			else if (message->StatusMsg() == IMidiMsg::kNoteOff)
			{
				for (int i = 0; i < numVoices; i++)
				{
					if (voices[i].GetNote() == message->NoteNumber())
					{
						voices[i].Release();
					}
				}
			}

			midiQueue.Remove();
		}

		double out = 0.0;
		for (int i = 0; i < numVoices; i++)
		{
			out += .25 * voices[i].Next();
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

	for (int i = 0; i < numVoices; i++)
	{
		if (paramIdx == oscillator1Split) voices[i].SetOscillator1Split(value);
		if (paramIdx == oscillatorMix) voices[i].SetOscillatorMix(value);

		if (paramIdx == envelopeAttack) voices[i].SetEnvelopeAttack(value);
		if (paramIdx == envelopeDecay) voices[i].SetEnvelopeDecay(value);
		if (paramIdx == envelopeSustain) voices[i].SetEnvelopeSustain(value);
		if (paramIdx == envelopeRelease) voices[i].SetEnvelopeRelease(value);
	}
}
