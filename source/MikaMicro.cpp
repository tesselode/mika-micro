#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR((int)Parameters::NumParameters, 1, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", 1);

	for (int voice = 0; voice < numVoices; voice++)
	{
		volEnvStage[voice] = EnvelopeStages::Idle;
		volEnvValue[voice] = 0.0;
	}
}

MikaMicro::~MikaMicro() {}

void MikaMicro::FlushMidi(int s)
{
	while (!midiQueue.Empty())
	{
		auto message = midiQueue.Peek();
		if (message->mOffset > s) break;

		auto status = message->StatusMsg();
		if (status == IMidiMsg::kNoteOn && message->Velocity() == 0) status = IMidiMsg::kNoteOff;

		switch (status)
		{
		case IMidiMsg::kNoteOff:
			for (int voice = 0; voice < numVoices; voice++)
			{
				if (!IsReleased(voice) && note[voice] == message->NoteNumber())
				{
					volEnvStage[voice] = EnvelopeStages::Release;
				}
			}
			break;
		case IMidiMsg::kNoteOn:
			for (int voice = 0; voice < numVoices; voice++)
			{
				if (IsReleased(voice))
				{
					volEnvStage[voice] = EnvelopeStages::Attack;
					note[voice] = message->NoteNumber();
					frequency[voice] = pitchToFrequency(note[voice]);
					phase[voice] = 0.0;
					break;
				}
			}
			break;
		}

		midiQueue.Remove();
	}
}

void MikaMicro::UpdateEnvelopes()
{
	for (int voice = 0; voice < numVoices; voice++)
	{
		switch (volEnvStage[voice])
		{
		case EnvelopeStages::Attack:
			volEnvValue[voice] += (1.1 - volEnvValue[voice]) * 10.0 * dt;
			if (volEnvValue[voice] >= 1.0)
			{
				volEnvValue[voice] = 1.0;
				volEnvStage[voice] = EnvelopeStages::Decay;
			}
			break;
		case EnvelopeStages::Decay:
			volEnvValue[voice] += (.5 - volEnvValue[voice]) * 10.0 * dt;
			break;
		case EnvelopeStages::Release:
			volEnvValue[voice] += (-.1 - volEnvValue[voice]) * 10.0 * dt;
			if (volEnvValue[voice] <= 0.0)
			{
				volEnvValue[voice] = 0.0;
				volEnvStage[voice] = EnvelopeStages::Idle;
			}
		}
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		UpdateEnvelopes();
		auto out = 0.0;
		for (int voice = 0; voice < numVoices; voice++)
		{
			if (volEnvStage[voice] != EnvelopeStages::Idle)
			{
				phase[voice] += frequency[voice] * dt;
				while (phase[voice] > 1.0) phase[voice] -= 1.0;
				out += .25 * sin(phase[voice] * twoPi) * volEnvValue[voice];
			}
		}
		outputs[0][s] = out;
		outputs[1][s] = out;
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
}
