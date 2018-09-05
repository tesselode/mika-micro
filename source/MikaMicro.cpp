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

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		*out1 = *in1;
		*out2 = *in2;
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
