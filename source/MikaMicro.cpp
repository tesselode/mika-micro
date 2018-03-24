#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(kOsc1Wave)->InitEnum("Oscillator 1 waveform", kWaveformSaw, kNumWaveforms);
	GetParam(kOsc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	GetParam(kOsc2Wave)->InitEnum("Oscillator 2 waveform", kWaveformSaw, kNumWaveforms);
	GetParam(kOsc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	GetParam(kOscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam(kFmMode)->InitEnum("FM mode", 0, 3);
	GetParam(kFmCoarse)->InitInt("FM coarse", 0, 0, 48);
	GetParam(kFmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam(kFilterEnabled)->InitBool("Filter enabled", false);
	GetParam(kFilterF)->InitDouble("Filter cutoff", 1.0, .001, 1.0, .01);
	GetParam(kFilterRes)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam(kFilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(kVolEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(kVolEnvR)->InitDouble("Volume envelope release", 925.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kModEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(kModEnvR)->InitDouble("Modulation envelope release", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kLfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(kLfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(kLfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(kVolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kLfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kVolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -1.0, 1.0, .01, "hz");
	GetParam(kModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -1.0, 1.0, .01, "hz");
	GetParam(kLfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -1.0, 1.0, .01);

	// master
	GetParam(kVoiceMode)->InitEnum("Voice mode", kVoiceModeLegato, kNumVoiceModes);
	GetParam(kGlideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(kMasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);
}

void MikaMicro::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 100);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 100);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 100);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto sliderBg = pGraphics->LoadIBitmap(SLIDERBG_ID, SLIDERBG_FN, 1);
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, kNumWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);
	auto fmModeSwitch = pGraphics->LoadIBitmap(FMMODESWITCH_ID, FMMODESWITCH_FN, 3);

	// oscillators
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, kOsc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, kOsc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, kOsc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, kOsc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, kOsc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, kOsc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, kOsc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, kOsc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, kOscMix, &slider));

	// fm
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, kFmMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, kFmCoarse, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, kFmFine, &knobMiddle));

	// filter
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 62 * 4, kFilterEnabled, &toggleSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, kFilterF, &knobRight));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, kFilterRes, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, kFilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, kVolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, kVolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, kVolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, kVolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, kVolEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, kModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, kModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, kModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, kModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, kModEnvV, &slider));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, kLfoAmount, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, kLfoFrequency, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, kLfoDelay, &knobLeft));

	// targets
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, kVolEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, kModEnvFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, kLfoFm, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, kVolEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, kModEnvCutoff, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, kLfoCutoff, &knobMiddle));

	// master
	pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, kVoiceMode, &fmModeSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, kGlideSpeed, &knobLeft));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, kMasterVolume, &knobLeft));

	AttachGraphics(pGraphics);
}

void MikaMicro::InitPresets()
{
	MakeDefaultPreset((char *) "Init", 1);
	MakePresetFromNamedParams("Tessa Bass", 38,
		kOsc1Wave, 2,
		kOsc1Coarse, -12,
		kOsc1Fine, 0.000000,
		kOsc1Split, 0.000000,
		kOsc2Wave, 3,
		kOsc2Coarse, -12,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 0.388889,
		kFmMode, 0,
		kFmCoarse, 0,
		kFmFine, 0.000000,
		kFilterEnabled, true,
		kFilterF, 0.219531,
		kFilterRes, 0.960938,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 965.953361,
		kVolEnvV, 0.000000,
		kModEnvA, 0.500000,
		kModEnvD, 998.000000,
		kModEnvS, 0.194444,
		kModEnvR, 998.000000,
		kModEnvV, 1.000000,
		kLfoAmount, 0.000000,
		kLfoFrequency, 4.000000,
		kLfoDelay, 0.100000,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.671875,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 2,
		kGlideSpeed, 842.428610,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("FACE EATER 8092", 38,
		kOsc1Wave, 2,
		kOsc1Coarse, -24,
		kOsc1Fine, 0.000000,
		kOsc1Split, 0.000000,
		kOsc2Wave, 3,
		kOsc2Coarse, 24,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 0.402778,
		kFmMode, 2,
		kFmCoarse, 2,
		kFmFine, 0.000000,
		kFilterEnabled, false,
		kFilterF, 1.000000,
		kFilterRes, 0.000000,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 925.000000,
		kVolEnvV, 0.000000,
		kModEnvA, 984.796864,
		kModEnvD, 990.929885,
		kModEnvS, 0.000000,
		kModEnvR, 998.000000,
		kModEnvV, 0.000000,
		kLfoAmount, 0.012500,
		kLfoFrequency, 1.819094,
		kLfoDelay, 998.825079,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 16.875000,
		kModEnvCutoff, 0.000000,
		kLfoFm, 2.062500,
		kLfoCutoff, 0.000000,
		kVoiceMode, 1,
		kGlideSpeed, 1.000000,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("Apex Twin", 38,
		kOsc1Wave, 1,
		kOsc1Coarse, -12,
		kOsc1Fine, 0.000000,
		kOsc1Split, 0.000000,
		kOsc2Wave, 2,
		kOsc2Coarse, 0,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 1.000000,
		kFmMode, 1,
		kFmCoarse, 23,
		kFmFine, 0.140625,
		kFilterEnabled, true,
		kFilterF, 0.847809,
		kFilterRes, 0.945313,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 925.000000,
		kVolEnvV, 0.000000,
		kModEnvA, 939.797718,
		kModEnvD, 998.000000,
		kModEnvS, 0.500000,
		kModEnvR, 998.000000,
		kModEnvV, 1.000000,
		kLfoAmount, 0.000000,
		kLfoFrequency, 4.000000,
		kLfoDelay, 0.100000,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, -1.000000,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 2,
		kGlideSpeed, 917.397440,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("Rubber Bass", 38,
		kOsc1Wave, 2,
		kOsc1Coarse, -24,
		kOsc1Fine, 0.000000,
		kOsc1Split, 0.001367,
		kOsc2Wave, 3,
		kOsc2Coarse, -24,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 0.500000,
		kFmMode, 1,
		kFmCoarse, 48,
		kFmFine, -0.289063,
		kFilterEnabled, false,
		kFilterF, 1.000000,
		kFilterRes, 0.000000,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 925.000000,
		kVolEnvV, 0.000000,
		kModEnvA, 998.000000,
		kModEnvD, 998.000000,
		kModEnvS, 0.500000,
		kModEnvR, 998.000000,
		kModEnvV, 0.000000,
		kLfoAmount, 0.000000,
		kLfoFrequency, 4.000000,
		kLfoDelay, 0.100000,
		kVolEnvFm, 22.687500,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.000000,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 2,
		kGlideSpeed, 1.000000,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("Super Strings", 38,
		kOsc1Wave, 2,
		kOsc1Coarse, 0,
		kOsc1Fine, 0.000000,
		kOsc1Split, -0.001563,
		kOsc2Wave, 2,
		kOsc2Coarse, 0,
		kOsc2Fine, 0.000000,
		kOsc2Split, -0.003711,
		kOscMix, 0.416667,
		kFmMode, 0,
		kFmCoarse, 0,
		kFmFine, 0.000000,
		kFilterEnabled, true,
		kFilterF, 0.449770,
		kFilterRes, 0.000000,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 969.681172,
		kVolEnvD, 998.937107,
		kVolEnvS, 0.666667,
		kVolEnvR, 997.828166,
		kVolEnvV, 0.333333,
		kModEnvA, 0.500000,
		kModEnvD, 998.000000,
		kModEnvS, 1.000000,
		kModEnvR, 998.000000,
		kModEnvV, 1.000000,
		kLfoAmount, 0.016406,
		kLfoFrequency, 4.000000,
		kLfoDelay, 0.100000,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.515625,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 0,
		kGlideSpeed, 1.000000,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("Echo Chrome", 38,
		kOsc1Wave, 1,
		kOsc1Coarse, 0,
		kOsc1Fine, -0.093750,
		kOsc1Split, -0.002539,
		kOsc2Wave, 0,
		kOsc2Coarse, 0,
		kOsc2Fine, 0.000000,
		kOsc2Split, -0.005273,
		kOscMix, 0.486111,
		kFmMode, 1,
		kFmCoarse, 4,
		kFmFine, 0.000000,
		kFilterEnabled, true,
		kFilterF, 0.422453,
		kFilterRes, 0.953125,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 983.502289,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 998.937107,
		kVolEnvV, 0.458333,
		kModEnvA, 998.000000,
		kModEnvD, 998.000000,
		kModEnvS, 0.500000,
		kModEnvR, 998.000000,
		kModEnvV, 0.000000,
		kLfoAmount, 0.006250,
		kLfoFrequency, 0.443384,
		kLfoDelay, 0.100000,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.000000,
		kLfoFm, 2.062500,
		kLfoCutoff, 0.000000,
		kVoiceMode, 0,
		kGlideSpeed, 1.000000,
		kMasterVolume, 0.289063);
	MakePresetFromNamedParams("Off-Brand PWM", 38,
		kOsc1Wave, 4,
		kOsc1Coarse, 0,
		kOsc1Fine, -0.226563,
		kOsc1Split, -0.000977,
		kOsc2Wave, 2,
		kOsc2Coarse, 0,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 1.000000,
		kFmMode, 1,
		kFmCoarse, 3,
		kFmFine, 0.000000,
		kFilterEnabled, false,
		kFilterF, 0.832199,
		kFilterRes, 0.000000,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 998.000000,
		kVolEnvS, 1.000000,
		kVolEnvR, 976.676699,
		kVolEnvV, 0.000000,
		kModEnvA, 998.000000,
		kModEnvD, 998.000000,
		kModEnvS, 0.500000,
		kModEnvR, 998.000000,
		kModEnvV, 0.000000,
		kLfoAmount, -0.015625,
		kLfoFrequency, 4.868913,
		kLfoDelay, 998.499152,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.000000,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 2,
		kGlideSpeed, 867.923270,
		kMasterVolume, 0.250000);
	MakePresetFromNamedParams("Christine", 38,
		kOsc1Wave, 0,
		kOsc1Coarse, 12,
		kOsc1Fine, 0.000000,
		kOsc1Split, 0.002344,
		kOsc2Wave, 0,
		kOsc2Coarse, 20,
		kOsc2Fine, 0.000000,
		kOsc2Split, 0.000000,
		kOscMix, 0.513889,
		kFmMode, 1,
		kFmCoarse, 15,
		kFmFine, 0.000000,
		kFilterEnabled, false,
		kFilterF, 1.000000,
		kFilterRes, 0.000000,
		kFilterKeyTrack, 0.000000,
		kVolEnvA, 0.500000,
		kVolEnvD, 999.296328,
		kVolEnvS, 0.000000,
		kVolEnvR, 999.296328,
		kVolEnvV, 0.000000,
		kModEnvA, 998.000000,
		kModEnvD, 998.000000,
		kModEnvS, 0.500000,
		kModEnvR, 998.000000,
		kModEnvV, 0.000000,
		kLfoAmount, 0.010938,
		kLfoFrequency, 4.000000,
		kLfoDelay, 0.100000,
		kVolEnvFm, 0.000000,
		kVolEnvCutoff, 0.000000,
		kModEnvFm, 0.000000,
		kModEnvCutoff, 0.000000,
		kLfoFm, 0.000000,
		kLfoCutoff, 0.000000,
		kVoiceMode, 0,
		kGlideSpeed, 1.000000,
		kMasterVolume, 0.250000);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParameters, 128, instanceInfo),
	gen(rd()),
	dist(-1.0, 1.0)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

MikaMicro::~MikaMicro()
{
	if (false) // set to true to save parameters on exit
	{
		const char* paramEnumNames[] =
		{
			"kOsc1Wave",
			"kOsc1Coarse",
			"kOsc1Fine",
			"kOsc1Split",
			"kOsc2Wave",
			"kOsc2Coarse",
			"kOsc2Fine",
			"kOsc2Split",
			"kOscMix",
			"kFmMode",
			"kFmCoarse",
			"kFmFine",
			"kFilterEnabled",
			"kFilterF",
			"kFilterRes",
			"kFilterKeyTrack",
			"kVolEnvA",
			"kVolEnvD",
			"kVolEnvS",
			"kVolEnvR",
			"kVolEnvV",
			"kModEnvA",
			"kModEnvD",
			"kModEnvS",
			"kModEnvR",
			"kModEnvV",
			"kLfoAmount",
			"kLfoFrequency",
			"kLfoDelay",
			"kVolEnvFm",
			"kVolEnvCutoff",
			"kModEnvFm",
			"kModEnvCutoff",
			"kLfoFm",
			"kLfoCutoff",
			"kVoiceMode",
			"kGlideSpeed",
			"kMasterVolume",
		};
		DumpPresetSrcCode("preset.txt", paramEnumNames);
	}
}

void MikaMicro::PlayVoices(int s)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > s) break;

		auto status = message->StatusMsg();
		auto note = message->NoteNumber();
		auto velocity = pow(message->Velocity() * .0078125, 1.25);
		bool noteOff = status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && velocity == 0);
		auto voiceMode = (EVoiceModes)(int)GetParam(kVoiceMode)->Value();
		bool mono = voiceMode == kVoiceModeMono || voiceMode == kVoiceModeLegato;

		if (noteOff)
		{
			heldNotes.erase(
				std::remove(
					std::begin(heldNotes),
					std::end(heldNotes),
					note),
				std::end(heldNotes));

			switch (voiceMode)
			{
			case kVoiceModePoly:
				for (auto& voice : voices)
					if (voice.GetNote() == note) voice.Release();
				break;
			case kVoiceModeMono:
			case kVoiceModeLegato:
				if (heldNotes.empty())
					voices[0].Release();
				else
					voices[0].SetNote(heldNotes.back());
				break;
			}
		}
		else if (status == IMidiMsg::kNoteOn)
		{
			switch (voiceMode)
			{
			case kVoiceModePoly:
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
				voice->SetVelocity(velocity);
				voice->ResetPitch();
				voice->Start();
				break;
			}
			case kVoiceModeMono:
				voices[0].SetNote(note);
				voices[0].SetVelocity(velocity);
				voices[0].Start();
				break;
			case kVoiceModeLegato:
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
		}
		else if (status == IMidiMsg::kPitchWheel)
		{
			auto f = pitchFactor(message->PitchWheel() * 2);
			for (auto& voice : voices) voice.SetPitchBendFactor(f);
		}
		else if (status == IMidiMsg::kAllNotesOff)
		{
			for (auto& voice : voices) voice.Release();
		}
		midiQueue.Remove();
	}
}

double MikaMicro::GetDriftValue()
{
	driftVelocity += dist(gen) * 10000 * dt;
	driftVelocity -= driftVelocity * 2 * dt;
	driftPhase += driftVelocity * dt;
	return .001 * sin(driftPhase);
}

double MikaMicro::GetVoices()
{
	auto lfoValue = lfo.Next(GetParam(kLfoFrequency)->Value(), kWaveformSine);
	auto driftValue = GetDriftValue();
	auto out = 0.0;
	for (auto &voice : voices) out += voice.Next(lfoValue, driftValue);
	return out * GetParam(kMasterVolume)->Value();
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		PlayVoices(s);
		auto out = GetVoices();
		*out1 = out;
		*out2 = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
	lfo.SetSampleRate(GetSampleRate());
	for (auto &voice : voices) voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::GrayOutControls()
{
	auto fmEnabled = (GetParam(kFmMode)->Value() == 1 && GetParam(kOscMix)->Value() > 0.0) ||
		(GetParam(kFmMode)->Value() == 2 && GetParam(kOscMix)->Value() < 1.0);
	auto osc1Enabled = GetParam(kOscMix)->Value() > 0.0;
	auto osc2Enabled = GetParam(kOscMix)->Value() < 1.0;
	auto isFilterEnabled = GetParam(kFilterEnabled)->Value();
	auto modEnvEnabled = GetParam(kModEnvFm)->Value() != 0.0 || GetParam(kModEnvCutoff)->Value() != 0.0;
	auto vibratoEnabled = GetParam(kLfoFm)->Value() != 0.0 || GetParam(kLfoCutoff)->Value() != 0.0 ||
		GetParam(kLfoAmount)->Value() < 0.0 || (GetParam(kLfoAmount)->Value() > 0.0 && osc2Enabled);

	pGraphics->GetControl(1)->GrayOut(!osc1Enabled);
	pGraphics->GetControl(2)->GrayOut(!(osc1Enabled || fmEnabled));
	pGraphics->GetControl(3)->GrayOut(!(osc1Enabled || fmEnabled));
	pGraphics->GetControl(4)->GrayOut(!osc1Enabled);
	for (int i = 5; i < 9; i++) pGraphics->GetControl(i)->GrayOut(!osc2Enabled);
	for (int i = 12; i < 14; i++) pGraphics->GetControl(i)->GrayOut(!fmEnabled);
	for (int i = 15; i < 18; i++) pGraphics->GetControl(i)->GrayOut(!isFilterEnabled);
	for (int i = 28; i < 38; i++) pGraphics->GetControl(i)->GrayOut(!modEnvEnabled);
	for (int i = 39; i < 41; i++) pGraphics->GetControl(i)->GrayOut(!vibratoEnabled);
	for (int i = 41; i < 44; i++) pGraphics->GetControl(i)->GrayOut(!fmEnabled);
	for (int i = 44; i < 47; i++) pGraphics->GetControl(i)->GrayOut(!isFilterEnabled);
	pGraphics->GetControl(48)->GrayOut(!GetParam(kVoiceMode)->Value());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	auto value = GetParam(paramIdx)->Value();

	// reverse parameters
	switch (paramIdx)
	{
	case kOscMix:
	case kVolEnvA:
	case kVolEnvD:
	case kVolEnvR:
	case kModEnvA:
	case kModEnvD:
	case kModEnvR:
	case kLfoDelay:
	case kGlideSpeed:
		value = GetParam(paramIdx)->GetMax() + GetParam(paramIdx)->GetMin() - value;
	}

	switch (paramIdx)
	{
	case kOsc1Wave:
		for (auto &voice : voices) voice.SetOsc1Wave((EWaveforms)(int)value);
		break;
	case kOsc1Coarse:
	case kOsc1Fine:
		for (auto &voice : voices)
			voice.SetOsc1Tune(GetParam(kOsc1Coarse)->Value() + GetParam(kOsc1Fine)->Value());
		break;
	case kOsc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(value);
		break;
	case kOsc2Wave:
		for (auto &voice : voices) voice.SetOsc2Wave((EWaveforms)(int)value);
		break;
	case kOsc2Coarse:
	case kOsc2Fine:
		for (auto &voice : voices)
			voice.SetOsc2Tune(GetParam(kOsc2Coarse)->Value() + GetParam(kOsc2Fine)->Value());
		break;
	case kOsc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(value);
		break;
	case kOscMix:
		for (auto &voice : voices) voice.SetOscMix(value);
		break;
	case kFmMode:
		for (auto &voice : voices) voice.SetFmMode(value);
		break;
	case kFmCoarse:
		for (auto &voice : voices) voice.SetFmCoarse(value);
		break;
	case kFmFine:
		for (auto &voice : voices) voice.SetFmFine(value);
		break;
	case kFilterEnabled:
		for (auto &voice : voices) voice.SetFilterEnabled(value);
		break;
	case kFilterF:
		for (auto &voice : voices) voice.SetFilterF(value);
		break;
	case kFilterRes:
		for (auto &voice : voices) voice.SetFilterRes(value);
		break;
	case kFilterKeyTrack:
		for (auto &voice : voices) voice.SetFilterKeyTrack(value);
		break;
	case kVolEnvA:
		for (auto &voice : voices) voice.SetVolumeEnvelopeAttack(value);
		break;
	case kVolEnvD:
		for (auto &voice : voices) voice.SetVolumeEnvelopeDecay(value);
		break;
	case kVolEnvS:
		for (auto &voice : voices) voice.SetVolumeEnvelopeSustain(value);
		break;
	case kVolEnvR:
		for (auto &voice : voices) voice.SetVolumeEnvelopeRelease(value);
		break;
	case kVolEnvV:
		for (auto &voice : voices) voice.SetVolumeEnvelopeVelocitySensitivity(value);
		break;
	case kModEnvA:
		for (auto &voice : voices) voice.SetModEnvelopeAttack(value);
		break;
	case kModEnvD:
		for (auto &voice : voices) voice.SetModEnvelopeDecay(value);
		break;
	case kModEnvS:
		for (auto &voice : voices) voice.SetModEnvelopeSustain(value);
		break;
	case kModEnvR:
		for (auto &voice : voices) voice.SetModEnvelopeRelease(value);
		break;
	case kModEnvV:
		for (auto &voice : voices) voice.SetModEnvelopeVelocitySensitivity(value);
		break;
	case kLfoAmount:
		for (auto &voice : voices) voice.SetLfoAmount(value);
		break;
	case kLfoDelay:
		for (auto &voice : voices) voice.SetLfoDelay(value);
		break;
	case kVolEnvFm:
		for (auto &voice : voices) voice.SetVolumeEnvelopeFm(value);
		break;
	case kModEnvFm:
		for (auto &voice : voices) voice.SetModEnvelopeFm(value);
		break;
	case kLfoFm:
		for (auto &voice : voices) voice.SetLfoFm(value);
		break;
	case kVolEnvCutoff:
		for (auto &voice : voices) voice.SetVolumeEnvelopeCutoff(value);
		break;
	case kModEnvCutoff:
		for (auto &voice : voices) voice.SetModEnvelopeCutoff(value);
		break;
	case kLfoCutoff:
		for (auto &voice : voices) voice.SetLfoCutoff(value);
		break;
	case kVoiceMode:
		for (int i = 1; i < voices.size(); i++) voices[i].Release();
		break;
	case kGlideSpeed:
		for (auto &voice : voices) voice.SetGlideSpeed(value);
		break;
	}

	GrayOutControls();
}

void MikaMicro::ProcessMidiMsg(IMidiMsg * message)
{
	midiQueue.Add(message);
}
