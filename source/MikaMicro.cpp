#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitPublicParameters()
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

	GetParam((int)PublicParameters::FilterMode)->InitEnum("Filter mode", (int)FilterModes::Off, (int)FilterModes::NumFilterModes);
	GetParam((int)PublicParameters::FilterMode)->SetDisplayText((int)FilterModes::Off, "Off");
	GetParam((int)PublicParameters::FilterMode)->SetDisplayText((int)FilterModes::TwoPole, "Two pole");
	GetParam((int)PublicParameters::FilterMode)->SetDisplayText((int)FilterModes::StateVariable, "State variable");
	GetParam((int)PublicParameters::FilterMode)->SetDisplayText((int)FilterModes::FourPole, "Four pole");
	GetParam((int)PublicParameters::FilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam((int)PublicParameters::FilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::FilterKeyTracking)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	GetParam((int)PublicParameters::VolEnvA)->InitDouble("Volume envelope attack time", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvD)->InitDouble("Volume envelope decay time", 0.5, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvR)->InitDouble("Volume envelope release time", 0.25, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::VolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::ModEnvA)->InitDouble("Mod envelope attack time", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::ModEnvD)->InitDouble("Mod envelope decay time", 0.5, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::ModEnvS)->InitDouble("Mod envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::ModEnvR)->InitDouble("Mod envelope release time", 0.25, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::ModEnvV)->InitDouble("Mod envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::LfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam((int)PublicParameters::LfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam((int)PublicParameters::LfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	GetParam((int)PublicParameters::VolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)PublicParameters::ModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)PublicParameters::LfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam((int)PublicParameters::VolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam((int)PublicParameters::ModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam((int)PublicParameters::LfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	GetParam((int)PublicParameters::VoiceMode)->InitEnum("Voice mode", (int)VoiceModes::Legato, (int)VoiceModes::NumVoiceModes);
	GetParam((int)PublicParameters::GlideLength)->InitDouble("Glide length", 0.0, 0.0, 1.0, .01);
	GetParam((int)PublicParameters::Volume)->InitDouble("Master volume", 0.5, 0.0, 1.0, .01);
}

void MikaMicro::InitInternalParameters()
{
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

	parameters[(int)InternalParameters::FilterDryMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterMode));
	parameters[(int)InternalParameters::FilterDryMix]->SetTransformation([](double v) {
		return (FilterModes)(int)v == FilterModes::Off ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Filter2pMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterMode));
	parameters[(int)InternalParameters::Filter2pMix]->SetTransformation([](double v) {
		return (FilterModes)(int)v == FilterModes::TwoPole ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::FilterSvfMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterMode));
	parameters[(int)InternalParameters::FilterSvfMix]->SetTransformation([](double v) {
		return (FilterModes)(int)v == FilterModes::StateVariable ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::Filter4pMix] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterMode));
	parameters[(int)InternalParameters::Filter4pMix]->SetTransformation([](double v) {
		return (FilterModes)(int)v == FilterModes::FourPole ? 1.0 : 0.0;
	});
	parameters[(int)InternalParameters::FilterCutoff] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterCutoff));
	parameters[(int)InternalParameters::FilterResonance] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterResonance));
	parameters[(int)InternalParameters::FilterKeyTracking] = std::make_unique<Parameter>(GetParam((int)PublicParameters::FilterKeyTracking));

	parameters[(int)InternalParameters::VolEnvA] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvA));
	parameters[(int)InternalParameters::VolEnvA]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvD] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvD));
	parameters[(int)InternalParameters::VolEnvD]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvS] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvS));
	parameters[(int)InternalParameters::VolEnvR] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvR));
	parameters[(int)InternalParameters::VolEnvR]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::VolEnvV] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvV));
	parameters[(int)InternalParameters::ModEnvA] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvA));
	parameters[(int)InternalParameters::ModEnvA]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::ModEnvD] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvD));
	parameters[(int)InternalParameters::ModEnvD]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::ModEnvS] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvS));
	parameters[(int)InternalParameters::ModEnvR] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvR));
	parameters[(int)InternalParameters::ModEnvR]->SetTransformation(envelopeCurve);
	parameters[(int)InternalParameters::ModEnvV] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvV));
	parameters[(int)InternalParameters::LfoAmount] = std::make_unique<Parameter>(GetParam((int)PublicParameters::LfoAmount));
	parameters[(int)InternalParameters::LfoFrequency] = std::make_unique<Parameter>(GetParam((int)PublicParameters::LfoFrequency));
	parameters[(int)InternalParameters::LfoDelay] = std::make_unique<Parameter>(GetParam((int)PublicParameters::LfoDelay));
	parameters[(int)InternalParameters::LfoDelay]->SetTransformation([](double v) {
		return 1000.1 - v;
	});

	parameters[(int)InternalParameters::VolEnvFm] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvFm));
	parameters[(int)InternalParameters::VolEnvCutoff] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VolEnvCutoff));
	parameters[(int)InternalParameters::ModEnvFm] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvFm));
	parameters[(int)InternalParameters::ModEnvCutoff] = std::make_unique<Parameter>(GetParam((int)PublicParameters::ModEnvCutoff));
	parameters[(int)InternalParameters::LfoFm] = std::make_unique<Parameter>(GetParam((int)PublicParameters::LfoFm));
	parameters[(int)InternalParameters::LfoCutoff] = std::make_unique<Parameter>(GetParam((int)PublicParameters::LfoCutoff));

	parameters[(int)InternalParameters::VoiceMode] = std::make_unique<Parameter>(GetParam((int)PublicParameters::VoiceMode));
	parameters[(int)InternalParameters::VoiceMode]->DisableSmoothing();
	parameters[(int)InternalParameters::GlideLength] = std::make_unique<Parameter>(GetParam((int)PublicParameters::GlideLength));
	parameters[(int)InternalParameters::GlideLength]->SetTransformation([](double v) {
		return 1000.0 - 998.0 * pow(v, .025);
	});
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
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 22 * 4, 62 * 4, (int)PublicParameters::FilterMode, &toggleSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, (int)PublicParameters::FilterCutoff, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, (int)PublicParameters::FilterResonance, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, (int)PublicParameters::FilterKeyTracking, &knobMiddle));

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
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, (int)PublicParameters::ModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, (int)PublicParameters::ModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, (int)PublicParameters::ModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, (int)PublicParameters::ModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, (int)PublicParameters::ModEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, (int)PublicParameters::LfoAmount, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, (int)PublicParameters::LfoFrequency, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, (int)PublicParameters::LfoDelay, &knobLeft));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, (int)PublicParameters::VolEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, (int)PublicParameters::ModEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, (int)PublicParameters::LfoFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, (int)PublicParameters::VolEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, (int)PublicParameters::ModEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, (int)PublicParameters::LfoCutoff, &knobMiddle));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, (int)PublicParameters::VoiceMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, (int)PublicParameters::GlideLength, &knobLeft));
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

	InitPublicParameters();
	InitInternalParameters();
	InitGraphics();
	InitPresets();
	InitVoices();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::FlushMidi(int sample)
{
	while (!midiQueue.Empty())
	{
		auto message = midiQueue.Peek();
		if (message->mOffset > sample) break;

		auto voiceMode = (VoiceModes)(int)parameters[(int)InternalParameters::VoiceMode]->Get();
		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = pow(message->Velocity() * .0078125, 1.25);

		if (status == IMidiMsg::kNoteOn && velocity == 0) status = IMidiMsg::kNoteOff;

		switch (status)
		{
		case IMidiMsg::kNoteOff:
			heldNotes.erase(
				std::remove(
					std::begin(heldNotes),
					std::end(heldNotes),
					note
				),
				std::end(heldNotes)
			);

			switch (voiceMode)
			{
			case VoiceModes::Poly:
				for (auto &voice : voices)
					if (voice.GetNote() == note) voice.Release();
				break;
			case VoiceModes::Mono:
			case VoiceModes::Legato:
				if (heldNotes.empty())
					voices[0].Release();
				else
					voices[0].SetNote(heldNotes.back());
				break;
			}
			break;
		case IMidiMsg::kNoteOn:
			switch (voiceMode)
			{
			case VoiceModes::Poly:
			{
				// get the quietest voice, prioritizing voices that are released
				auto voice = std::min_element(
					std::begin(voices),
					std::end(voices),
					[](Voice a, Voice b)
				{
					return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
				}
				);
				voice->SetNote(note);
				voice->SetVelocity(velocity);
				voice->ResetPitch();
				voice->Start();
				break;
			}
			case VoiceModes::Mono:
				voices[0].SetNote(note);
				voices[0].SetVelocity(velocity);
				voices[0].Start();
				break;
			case VoiceModes::Legato:
				voices[0].SetNote(note);
				if (heldNotes.empty())
				{
					voices[0].SetVelocity(velocity);
					voices[0].ResetPitch();
					voices[0].Start();
				}
				break;
			}

			heldNotes.push_back(note);
			break;
		case IMidiMsg::kPitchWheel:
		{
			auto pitchBendFactor = pitchFactor(message->PitchWheel() * 2.0);
			for (auto &voice : voices) voice.SetPitchBendFactor(pitchBendFactor);
			break;
		}
		case IMidiMsg::kAllNotesOff:
			for (auto &voice : voices) voice.Release();
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
		auto lfoValue = lfo.Next(dt, parameters[(int)InternalParameters::LfoFrequency]->Get(), 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		auto out = 0.0;
		for (auto &v : voices) out += v.Next(dt, lfoValue);
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
