#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam((int)PublicParameters::Osc1Wave)->InitEnum("Oscillator 1 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)PublicParameters::Osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam((int)PublicParameters::Osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam((int)PublicParameters::Osc1Split)->InitDouble("Oscillator 1 split", 0.0, -1.0, 1.0, .01);
	GetParam((int)PublicParameters::Osc2Wave)->InitEnum("Oscillator 2 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)PublicParameters::Osc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam((int)PublicParameters::Osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam((int)PublicParameters::Osc2Split)->InitDouble("Oscillator 2 split", 0.0, -1.0, 1.0, .01);
	GetParam((int)PublicParameters::OscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	GetParam((int)PublicParameters::FmMode)->InitEnum("FM mode", (int)FmModes::Off, (int)FmModes::NumFmModes);
	GetParam((int)PublicParameters::FmCoarse)->InitInt("FM coarse", 0, 0, 48, "semitones");
	GetParam((int)PublicParameters::FmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01, "semitones");

	GetParam((int)PublicParameters::FilterEnabled)->InitBool("Filter enabled", false);
	GetParam((int)PublicParameters::FilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam((int)PublicParameters::FilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);

	GetParam((int)PublicParameters::VolEnvA)->InitDouble("Volume envelope attack time", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvD)->InitDouble("Volume envelope decay time", 0.5, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvR)->InitDouble("Volume envelope release time", 0.25, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::Volume)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);

	auto envelopeCurve = [](double v) {
		return 1000.0 - 999.9 * pow(.5 - .5 * cos(v * pi), .01);
	};

	parameters[(int)InternalParameters::Osc1SineMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1SineMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Sine ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1TriangleMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1TriangleMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Triangle ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1SawMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1SawMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Saw ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1SquareMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1SquareMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Square ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1PulseMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1PulseMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Pulse ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1NoiseMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Wave));
	parameters[(int)InternalParameters::Osc1NoiseMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Noise ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1Coarse] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Coarse));
	parameters[(int)InternalParameters::Osc1Coarse]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc1Fine] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Fine));
	parameters[(int)InternalParameters::Osc1Fine]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc1SplitMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Split));
	parameters[(int)InternalParameters::Osc1SplitMix]->SetTransformation([](double v) {
		return v != 0.0 ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc1SplitFactorA] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Split));
	parameters[(int)InternalParameters::Osc1SplitFactorA]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc1SplitFactorB] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc1Split));
	parameters[(int)InternalParameters::Osc1SplitFactorB]->SetTransformation([](double v) {
		return pitchFactor(-v);
	});
	
	parameters[(int)InternalParameters::Osc2SineMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2SineMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Sine ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2TriangleMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2TriangleMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Triangle ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2SawMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2SawMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Saw ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2SquareMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2SquareMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Square ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2PulseMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2PulseMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Pulse ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2NoiseMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Wave));
	parameters[(int)InternalParameters::Osc2NoiseMix]->SetTransformation([](double v) {
		return v == (int)Waveforms::Noise ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2Coarse] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Coarse));
	parameters[(int)InternalParameters::Osc2Coarse]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc2Fine] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Fine));
	parameters[(int)InternalParameters::Osc2Fine]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc2SplitMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Split));
	parameters[(int)InternalParameters::Osc2SplitMix]->SetTransformation([](double v) {
		return v != 0.0 ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Osc2SplitFactorA] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Split));
	parameters[(int)InternalParameters::Osc2SplitFactorA]->SetTransformation([](double v) {
		return pitchFactor(v);
	});
	parameters[(int)InternalParameters::Osc2SplitFactorB] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Osc2Split));
	parameters[(int)InternalParameters::Osc2SplitFactorB]->SetTransformation([](double v) {
		return pitchFactor(-v);
	});

	parameters[(int)InternalParameters::OscMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::OscMix));
	parameters[(int)InternalParameters::OscMix]->SetTransformation([](double v) {
		return 1.0 - v;
	});

	parameters[(int)InternalParameters::FmMode] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FmMode));
	parameters[(int)InternalParameters::FmMode]->DisableSmoothing();
	parameters[(int)InternalParameters::FmCoarse] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FmCoarse));
	parameters[(int)InternalParameters::FmFine] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FmFine));

	parameters[(int)InternalParameters::FilterMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterEnabled));
	parameters[(int)InternalParameters::FilterCutoff] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterCutoff));
	parameters[(int)InternalParameters::FilterResonance] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterResonance));

	parameters[(int)InternalParameters::VolEnvA] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvA));
	parameters[(int)InternalParameters::VolEnvA]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvD] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvD));
	parameters[(int)InternalParameters::VolEnvD]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvS] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvS));
	parameters[(int)InternalParameters::VolEnvR] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvR));
	parameters[(int)InternalParameters::VolEnvR]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvV] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvV));

	parameters[(int)InternalParameters::Volume] = std::make_unique<Parameter>(GetParam((int)PublicParameters::Volume));
}

void MikaMicro::InitGraphics()
{
	auto pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 100);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 100);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 100);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto sliderBg = pGraphics->LoadIBitmap(SLIDERBG_ID, SLIDERBG_FN, 1);
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, (int)Waveforms::NumWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);
	auto fmModeSwitch = pGraphics->LoadIBitmap(FMMODESWITCH_ID, FMMODESWITCH_FN, 3);

	// oscillators
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, (int)PublicParameters::Osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, (int)PublicParameters::Osc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, (int)PublicParameters::Osc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, (int)PublicParameters::Osc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, (int)PublicParameters::Osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, (int)PublicParameters::Osc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, (int)PublicParameters::Osc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, (int)PublicParameters::Osc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, (int)PublicParameters::OscMix, &slider));

	// fm
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, (int)PublicParameters::FmMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, (int)PublicParameters::FmCoarse, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, (int)PublicParameters::FmFine, &knobMiddle));

	// filter
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 62 * 4, (int)PublicParameters::FilterEnabled, &toggleSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, (int)PublicParameters::FilterCutoff, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, (int)PublicParameters::FilterResonance, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, (int)Parameters::FilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, (int)PublicParameters::VolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, (int)PublicParameters::VolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, (int)PublicParameters::VolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, (int)PublicParameters::VolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, (int)PublicParameters::VolEnvV, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvA, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvD, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvS, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvR, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, (int)Parameters::ModEnvV, &slider));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, (int)Parameters::LfoAmount, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, (int)Parameters::LfoFrequency, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, (int)Parameters::LfoDelay, &knobLeft));

	// targets
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, (int)Parameters::VolEnvFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, (int)Parameters::ModEnvFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, (int)Parameters::LfoFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, (int)Parameters::VolEnvCutoff, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, (int)Parameters::ModEnvCutoff, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, (int)Parameters::LfoCutoff, &knobMiddle));

	// master
	//pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, (int)Parameters::VoiceMode, &fmModeSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, (int)Parameters::GlideLength, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, (int)PublicParameters::Volume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

void MikaMicro::InitPresets()
{
	MakeDefaultPreset((char *) "-", 1);
}

void MikaMicro::InitVoices()
{
	for (int i = 0; i < numVoices; i++) voices.push_back(Voice(parameters));
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR((int)PublicParameters::NumParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
	InitVoices();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::FlushMidi(int s)
{
	while (!midiQueue.Empty())
	{
		auto message = midiQueue.Peek();
		if (message->mOffset > s) break;

		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = message->Velocity();

		if (status == IMidiMsg::kNoteOn && velocity == 0) status = IMidiMsg::kNoteOff;

		switch (status)
		{
		case IMidiMsg::kNoteOff:
			for (auto &v : voices)
				if (v.GetNote() == note) v.Release();
			break;
		case IMidiMsg::kNoteOn:
			// get the quietest voice, prioritizing voices that are released
			auto voice = std::min_element(std::begin(voices), std::end(voices), [](Voice a, Voice b) {
				return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
			});
			voice->SetNote(note);
			voice->Start();
			break;
		}

		midiQueue.Remove();
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		for (auto &p : parameters) p->Update(dt);
		auto out = 0.0;
		for (auto &v : voices) out += v.Next(dt);
		out *= parameters[(int)InternalParameters::Volume]->Get();
		outputs[0][s] = out;
		outputs[1][s] = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
}
