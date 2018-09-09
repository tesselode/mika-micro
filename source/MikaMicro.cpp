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
			for (int i = 0; i < numVoices; i++)
			{
				if (note[i] == message->NoteNumber())
				{
					isPlaying[i] = false;
				}
			}
			break;
		case IMidiMsg::kNoteOn:
			for (int i = 0; i < numVoices; i++)
			{
				if (!isPlaying[i])
				{
					isPlaying[i] = true;
					note[i] = message->NoteNumber();
					frequency[i] = pitchToFrequency(note[i]);
					phase[i] = 0.0;
					break;
				}
			}
			break;
		}

		midiQueue.Remove();
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		auto out = 0.0;
		for (int i = 0; i < numVoices; i++)
		{
			if (isPlaying[i])
			{
				phase[i] += frequency[i] * dt;
				while (phase[i] > 1.0) phase[i] -= 1.0;
				out += .25 * sin(phase[i] * twoPi);
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
