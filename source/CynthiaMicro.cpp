#include "CynthiaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

CynthiaMicro::CynthiaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(1, kNumPrograms, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", kNumPrograms);

	voice.SetNote(69);
	voice.Start();
}

CynthiaMicro::~CynthiaMicro() {}

void CynthiaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		double out = .25 * voice.Next();
		*out1 = out;
		*out2 = out;
	}
}

void CynthiaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);

	voice.SetSampleRate(GetSampleRate());
}

void CynthiaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
}
