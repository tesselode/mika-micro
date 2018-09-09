#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam((int)Parameters::Osc1Wave)->InitEnum("Oscillator 1 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)Parameters::Osc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam((int)Parameters::Osc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam((int)Parameters::Osc2Wave)->InitEnum("Oscillator 1 waveform", (int)Waveforms::Saw, (int)Waveforms::NumWaveforms);
	GetParam((int)Parameters::Osc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam((int)Parameters::Osc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");

	GetParam((int)Parameters::VolEnvA)->InitDouble("Volume envelope attack time", 0.5, 0.0, 1.0, .01);
	GetParam((int)Parameters::VolEnvD)->InitDouble("Volume envelope decay time", 0.5, 0.0, 1.0, .01);
	GetParam((int)Parameters::VolEnvS)->InitDouble("Volume envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam((int)Parameters::VolEnvR)->InitDouble("Volume envelope release time", 0.5, 0.0, 1.0, .01);
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
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
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, (int)Parameters::Osc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, (int)Parameters::Osc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, (int)Parameters::Osc1Fine, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, (int)Parameters::Osc1Split, &knobMiddle));
	pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, (int)Parameters::Osc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, (int)Parameters::Osc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, (int)Parameters::Osc2Fine, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, (int)Parameters::Osc2Split, &knobMiddle));
	//pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, (int)Parameters::OscMix, &slider));

	// fm
	//pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, (int)Parameters::FmMode, &fmModeSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, (int)Parameters::FmCoarse, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, (int)Parameters::FmFine, &knobMiddle));

	// filter
	//pGraphics->AttachControl(new ISwitchPopUpControl(this, 22 * 4, 62 * 4, (int)Parameters::FilterMode, &fmModeSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, (int)Parameters::FilterCutoff, &knobRight));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, (int)Parameters::FilterResonance, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, (int)Parameters::FilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvR, &slider));
	//pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, (int)Parameters::VolEnvV, &slider));
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
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, (int)Parameters::MasterVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

void MikaMicro::InitPresets()
{
	MakeDefaultPreset((char *) "-", 1);
}

void MikaMicro::InitVoices()
{
	for (int voice = 0; voice < numVoices; voice++)
	{
		volEnvStage[voice] = EnvelopeStages::Idle;
		volEnvValue[voice] = 0.0;
		note[voice] = 0;
		frequency[voice] = 0.0;
	}
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR((int)Parameters::NumParameters, 1, instanceInfo)
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
		if (status == IMidiMsg::kNoteOn && message->Velocity() == 0) status = IMidiMsg::kNoteOff;

		switch (status)
		{
		case IMidiMsg::kNoteOff:
			for (int voice = 0; voice < numVoices; voice++)
			{
				if (!IsReleased(voice) && note[voice] == message->NoteNumber())
				{
					volEnvStage[voice] = EnvelopeStages::Release;
				}
			}
			break;
		case IMidiMsg::kNoteOn:
			for (int voice = 0; voice < numVoices; voice++)
			{
				if (IsReleased(voice))
				{
					volEnvStage[voice] = EnvelopeStages::Attack;
					note[voice] = message->NoteNumber();
					frequency[voice] = pitchToFrequency(note[voice]);
					osc1[voice].phase = 0.0;
					break;
				}
			}
			break;
		}

		midiQueue.Remove();
	}
}

void MikaMicro::UpdateEnvelopes()
{
	for (int voice = 0; voice < numVoices; voice++)
	{
		switch (volEnvStage[voice])
		{
		case EnvelopeStages::Attack:
			volEnvValue[voice] += (1.1 - volEnvValue[voice]) * volEnvA * dt;
			if (volEnvValue[voice] >= 1.0)
			{
				volEnvValue[voice] = 1.0;
				volEnvStage[voice] = EnvelopeStages::Decay;
			}
			break;
		case EnvelopeStages::Decay:
			volEnvValue[voice] += (volEnvS - volEnvValue[voice]) * volEnvD * dt;
			break;
		case EnvelopeStages::Release:
			volEnvValue[voice] += (-.1 - volEnvValue[voice]) * volEnvR * dt;
			if (volEnvValue[voice] <= 0.0)
			{
				volEnvValue[voice] = 0.0;
				volEnvStage[voice] = EnvelopeStages::Idle;
			}
		}
	}
}

double MikaMicro::GetOscillator(Oscillator &osc, double frequency)
{
	osc.phaseIncrement = frequency * osc.tune * dt;
	osc.phase += osc.phaseIncrement;
	while (osc.phase > 1.0) osc.phase -= 1.0;

	switch (osc.waveform)
	{
	case Waveforms::Sine:
		return sin(osc.phase * twoPi);
	case Waveforms::Triangle:
		osc.triLast = osc.triCurrent;
		osc.triCurrent = osc.phaseIncrement * GeneratePulse(osc.phase, osc.phaseIncrement, .5) + (1.0 - osc.phaseIncrement) * osc.triLast;
		return osc.triCurrent * 5.0;
	case Waveforms::Saw:
		return 1.0 - 2.0 * osc.phase + Blep(osc.phase, osc.phaseIncrement);
		break;
	case Waveforms::Square:
		return GeneratePulse(osc.phase, osc.phaseIncrement, .5);
	case Waveforms::Pulse:
		return GeneratePulse(osc.phase, osc.phaseIncrement, .75);
	case Waveforms::Noise:
		osc.noiseValue += 19.0;
		osc.noiseValue *= osc.noiseValue;
		osc.noiseValue -= (int)osc.noiseValue;
		return osc.noiseValue - .5;
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		UpdateEnvelopes();
		auto out = 0.0;
		for (int voice = 0; voice < numVoices; voice++)
		{
			if (volEnvStage[voice] != EnvelopeStages::Idle)
			{
				out += .25 * GetOscillator(osc1[voice], frequency[voice]) * volEnvValue[voice];
				out += .25 * GetOscillator(osc2[voice], frequency[voice]) * volEnvValue[voice];
			}
		}
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

	auto parameter = (Parameters)paramIdx;
	auto value = GetParam(paramIdx)->Value();

	switch (parameter)
	{
	case Parameters::Osc1Wave:
		for (int voice = 0; voice < numVoices; voice++)
		{
			osc1[voice].waveform = (Waveforms)(int)value;
		}
		break;
	case Parameters::Osc1Coarse:
	case Parameters::Osc1Fine:
	{
		auto coarse = GetParam((int)Parameters::Osc1Coarse)->Value();
		auto fine = GetParam((int)Parameters::Osc1Fine)->Value();
		auto tune = pitchFactor(coarse + fine);
		for (int voice = 0; voice < numVoices; voice++)
		{
			osc1[voice].tune = tune;
		}
		break;
	}
	case Parameters::Osc2Wave:
		for (int voice = 0; voice < numVoices; voice++)
		{
			osc2[voice].waveform = (Waveforms)(int)value;
		}
		break;
	case Parameters::Osc2Coarse:
	case Parameters::Osc2Fine:
	{
		auto coarse = GetParam((int)Parameters::Osc2Coarse)->Value();
		auto fine = GetParam((int)Parameters::Osc2Fine)->Value();
		auto tune = pitchFactor(coarse + fine);
		for (int voice = 0; voice < numVoices; voice++)
		{
			osc2[voice].tune = tune;
		}
		break;
	}
	case Parameters::VolEnvA:
		volEnvA = 1000 - 999.9 * (.5 - .5 * cos(pow(value, .1) * pi));
		break;
	case Parameters::VolEnvD:
		volEnvD = 1000 - 999.9 * (.5 - .5 * cos(pow(value, .1) * pi));
		break;
	case Parameters::VolEnvS:
		volEnvS = value;
		break;
	case Parameters::VolEnvR:
		volEnvR = 1000 - 999.9 * (.5 - .5 * cos(pow(value, .1) * pi));
		break;
	}
}
