#include "MidiReceiver.h"

void MidiReceiver::SetMono(bool m)
{
	mono = m;
}

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
			heldNotes.erase(
				std::remove_if(
					std::begin(heldNotes),
					std::end(heldNotes),
					[note](auto n) { return n.note == note; }),
				std::end(heldNotes));

			if (mono)
			{
				if (heldNotes.empty())
					voices[0].Release();
				else
				{
					voices[0].SetNote(heldNotes.back().note);
				}
			}
			else
			{
				for (auto& voice : voices)
					if (voice.GetNote() == note) voice.Release();
			}
		}
		else if (status == IMidiMsg::kNoteOn)
		{
			if (mono)
			{
				voices[0].SetNote(note);
				if (heldNotes.empty()) voices[0].Start();
			}
			else
			{
				// get the quietest voice, prioritizing voices that are released
				auto voice = std::min_element(
					std::begin(voices),
					std::end(voices),
					[](Voice a, Voice b)
				{
					return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
				});
				voice->SetNote(note);
				voice->Start();
			}

			heldNotes.push_back(HeldNote{ note = note, velocity = velocity });
		}
		else if (status == IMidiMsg::kPitchWheel)
		{
			for (auto& voice : voices)
				voice.SetPitchBendFactor(pitchFactor(message->PitchWheel() * 2));
		}
		midiQueue.Remove();
	}
}