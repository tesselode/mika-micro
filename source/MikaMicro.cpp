#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam(volEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(volEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(volEnvR)->InitDouble("Volume envelope release", 100.0, 0.5, 1000.0, .01, "", "", .025);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	AttachGraphics(pGraphics);

	MakeDefaultPreset((char *) "-", 1);

	voices[0].SetNote(69);
	voices[0].Start();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Next();
		out *= .25;

		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	for (auto &voice : voices) voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	auto value = GetParam(paramIdx)->Value();

	// reverse parameters
	switch (paramIdx)
	{
	case volEnvA:
	case volEnvD:
	case volEnvR:
		value = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - value;
	}

	switch (paramIdx)
	{
	case volEnvA:
		for (auto &voice : voices) voice.SetVolumeEnvelopeAttack(value);
	case volEnvD:
		for (auto &voice : voices) voice.SetVolumeEnvelopeDecay(value);
	case volEnvS:
		for (auto &voice : voices) voice.SetVolumeEnvelopeSustain(value);
	case volEnvR:
		for (auto &voice : voices) voice.SetVolumeEnvelopeRelease(value);
	}
}
