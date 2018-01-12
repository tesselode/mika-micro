#ifndef __CYNTHIAMICRO__
#define __CYNTHIAMICRO__

#include "IPlug_include_in_plug_hdr.h"

class CynthiaMicro : public IPlug
{
public:
	CynthiaMicro(IPlugInstanceInfo instanceInfo);
	~CynthiaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
};

#endif
