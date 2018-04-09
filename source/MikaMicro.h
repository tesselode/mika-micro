#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitParameters();
	void InitGraphics();
	void GrayOutControls();

	double dt = 0.0;
	IGraphics* pGraphics;
	Voice voice;
};

#endif
