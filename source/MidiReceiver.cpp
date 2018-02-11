#include "MidiReceiver.h"

void MidiReceiver::Add(IMidiMsg * message)
{
	auto status = message->StatusMsg();
	if (status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff)
		midiQueue.Add(message);
}

void MidiReceiver::Process(std::vector<Voice>& voices, int s)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > s) break;
		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		if (status == IMidiMsg::kNoteOn)
		{
			// get the quietest voice, prioritizing voices that are released
			auto voice = std::min_element(std::begin(voices), std::end(voices),
				[](Voice a, Voice b)
			{
				return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
			});
			voice->SetNote(note);
			voice->Start();
		}
		else if (status == IMidiMsg::kNoteOff)
		{
			for (auto& voice : voices)
				if (voice.GetNote() == note) voice.Release();
		}
		midiQueue.Remove();
	}
}
