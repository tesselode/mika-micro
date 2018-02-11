#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 1.0, 0.1, 100.0, .01);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 1.0, 0.1, 100.0, .01);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 1.0, 0.1, 100.0, .01);

	for (int i = 0; i < numParameters; i++)
		parameters.push_back(GetParam(i)->Value());
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", 1);

	for (int i = 0; i < numVoices; i++) voices.push_back(Voice());
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	midiReceiver.Add(message);
}


void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		midiReceiver.Process(voices, s);

		double out = 0.0;
		for (auto& voice : voices)
			out += .25 * voice.Next(1.0 / GetSampleRate(), parameters);

		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
}

void MikaMicro::OnParamChange(int paramIdx)
{
	parameters[paramIdx] = GetParam(paramIdx)->Value();
	IMutexLock lock(this);
}