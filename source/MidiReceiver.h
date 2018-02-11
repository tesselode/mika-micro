#pragma once

#include <algorithm>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include <vector>
#include "Voice.h"

class MidiReceiver
{
public:
	void Add(IMidiMsg *message);
	void Process(std::vector<Voice> &voices, int s);

private:
	IMidiQueue midiQueue;
};

