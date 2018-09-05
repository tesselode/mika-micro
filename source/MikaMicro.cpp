#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam((int)Parameters::Volume)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);

	for (int i = 0; i < (int)Parameters::NumParameters; i++)
		parameters[i] = std::make_unique<Parameter>(GetParam(i));
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 100);
	pGraphics->AttachControl(new IKnobMultiControl(this, 50, 50, (int)Parameters::Volume, &knobLeft));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR((int)Parameters::NumParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	MakeDefaultPreset((char *) "-", 1);
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		while (!midiQueue.Empty())
		{
			auto message = midiQueue.Peek();
			if (message->mOffset > s) break;

			auto status = message->StatusMsg();
			auto note = message->NoteNumber();
			auto velocity = message->Velocity();

			if (status == IMidiMsg::kNoteOn && velocity == 0) status = IMidiMsg::kNoteOff;

			switch (status)
			{
			case IMidiMsg::kNoteOff:
				voice.Release();
				break;
			case IMidiMsg::kNoteOn:
				voice.SetNote(note);
				voice.Start();
				break;
			}

			midiQueue.Remove();
		}

		auto out = voice.Next(dt);
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
