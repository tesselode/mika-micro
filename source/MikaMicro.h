#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "Parameter.h"

enum class Parameters
{
	Volume,
	NumParameters
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
	void InitParameters();
	void InitGraphics();

	std::array<std::unique_ptr<Parameter>, (int)Parameters::NumParameters> parameters;
	double dt = 0.0;
	Oscillator osc;
	Envelope env;
};

#endif
