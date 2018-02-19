#include "MidiReceiver.h"

void MidiReceiver::Add(IMidiMsg * message)
{
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
		auto velocity = message->Velocity();
		bool noteOff = status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && velocity == 0);

		if (noteOff)
		{
			for (auto& voice : voices)
				if (voice.GetNote() == note) voice.Release();
		}
		else if (status == IMidiMsg::kNoteOn)
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
		else if (status == IMidiMsg::kPitchWheel)
		{
			for (auto& voice : voices)
				voice.SetPitchBendFactor(pitchFactor(message->PitchWheel() * 2));
		}
		midiQueue.Remove();
	}
}