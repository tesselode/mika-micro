#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

enum Parameters
{
	numParameters
};

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", 1);

	voice.SetNote(69);
	voice.Start();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		double out = 0.0;
		out += .25 * voice.Next(1.0 / GetSampleRate());

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
	IMutexLock lock(this);
}
