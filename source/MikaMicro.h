#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include "IPlug_include_in_plug_hdr.h"
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
	Voice voice;
};

#endif
