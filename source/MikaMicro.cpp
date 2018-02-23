#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(0, 1, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", 1);

	voice.SetVolumeEnvelopeAttack(1.0);
	voice.SetVolumeEnvelopeDecay(1.0);
	voice.SetVolumeEnvelopeSustain(0.5);
	voice.SetVolumeEnvelopeRelease(1.0);
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
		auto out = 0.0;
		out += .25 * voice.Next();

		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
}
