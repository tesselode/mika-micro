#pragma once

#include <algorithm>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Util.h"
#include <vector>
#include "Voice.h"

struct HeldNote
{
	int note;
	int velocity;
};

class MidiReceiver
{
public:
	void SetMono(bool m);
	void Add(IMidiMsg *message);
	void Process(std::vector<Voice> &voices, int s);

private:
	IMidiQueue midiQueue;
	bool mono = true;
	std::vector<HeldNote> heldNotes;
};

