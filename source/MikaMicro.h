#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "Voice.h"

enum ParameterList
{
	volEnvA,
	volEnvD,
	volEnvS,
	volEnvR,
	numParameters,
};

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	std::array<Voice, 8> voices;

	void InitParameters();
};

#endif
