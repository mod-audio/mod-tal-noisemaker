/*
	==============================================================================
	This file is part of Tal-NoiseMaker by Patrick Kunz.

	Copyright(c) 2005-2010 Patrick Kunz, TAL
	Togu Audio Line, Inc.
	http://kunz.corrupt.ch

	This file may be licensed under the terms of of the
	GNU General Public License Version 2 (the ``GPL'').

	Software distributed under the License is distributed
	on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
	express or implied. See the GPL for the specific language
	governing rights and limitations.

	You should have received a copy of the GPL along with this
	program. If not, go to http://www.gnu.org/licenses/gpl.html
	or write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
	==============================================================================
 */

#if !defined(__SynthEngine_h)
#define __SynthEngine_h

#include "AudioUtils.h"
#include "Params.h"
#include "SynthVoice.h"
#include "VoiceManager.h"
#include "ParamChangeUtil.h"
#include "LfoHandler1.h"
#include "LfoHandler2.h"
#include "PitchwheelHandler.h"
#include "VelocityHandler.h"
#include "HighPass.h"
#include "StereoPan.h"
#include "OscNoise.h"

class SynthEngine
{
private:
    float sampleRate;

	float volume;
	float cutoff;

  ScopedPointer<VoiceManager> voiceManager;
  ScopedPointer<ParamChangeUtil> cutoffFiltered;
  ScopedPointer<LfoHandler1> lfoHandler1;
  ScopedPointer<LfoHandler2> lfoHandler2;
  ScopedPointer<PitchwheelHandler> pitchwheelHandler;
  ScopedPointer<VelocityHandler> velocityHandler;
  ScopedPointer<HighPass> highPass;
  ScopedPointer<StereoPan> stereoPan;
  ScopedPointer<OscNoise> denormalNoise;

	AudioUtils audioUtils;

public:
	SynthEngine(float sampleRate)
	{
		initialize(sampleRate);
	}

	~SynthEngine()
	{
	}

private:
	void initialize(float sampleRate)
	{
        if (sampleRate <= 0)
        {
            sampleRate = 44100.0f;
        }

        this->sampleRate = sampleRate;
		this->cutoff = 1.0f;
		this->volume = 1.0f;

		cutoffFiltered = new ParamChangeUtil(sampleRate, 1000.0f);

		lfoHandler1 = new LfoHandler1(sampleRate);
		lfoHandler2 = new LfoHandler2(sampleRate);

        pitchwheelHandler = new PitchwheelHandler(sampleRate);
        velocityHandler = new VelocityHandler(sampleRate);

        highPass = new HighPass();
        this->stereoPan = new StereoPan(lfoHandler2);

        this->denormalNoise = new OscNoise(sampleRate);

		voiceManager = new VoiceManager(sampleRate, lfoHandler1, lfoHandler2, velocityHandler, pitchwheelHandler);
	}

	Osc::Waveform getOsc1Waveform(int value)
	{
		Osc::Waveform waveform;

		switch (value)
		{
        case 0: waveform = Osc::SAW; break;
        case 1: waveform = Osc::PULSE; break;
        case 2: waveform = Osc::NOISE; break;
        default: waveform = Osc::SAW;
		}
		return waveform;
	}

	Osc::Waveform getOsc2Waveform(int value)
	{
		Osc::Waveform waveform;

		switch (value)
		{
        case 0: waveform = Osc::SAW; break;
        case 1: waveform = Osc::PULSE; break;
        case 2: waveform = Osc::TRIANGLE; break;
        case 3: waveform = Osc::SIN; break;
        case 4: waveform = Osc::NOISE; break;
        default: waveform = Osc::SAW;
		}
		return waveform;
	}

public:
	void setSampleRate(float sampleRate)
	{
		initialize(sampleRate);
	}

	void setNumberOfVoices(int numberOfVoices)
	{
		//std::cout << "numberOfVoices in setNumber = " << numberOfVoices << std::endl;
		this->voiceManager->setNumberOfVoices(numberOfVoices);
	}

	void setNoteOn(int note, float velocity)
	{
		voiceManager->setNoteOn(note, velocity);
	}

	void setNoteOff(int note)
	{
		voiceManager->setNoteOff(note);
	}

	void setVolume(float value)
	{
		this->volume = audioUtils.getLogScaledVolume(value, 2.0f);
	}

	void setOsc1Volume(float value)
	{
		value = audioUtils.getLogScaledVolume(value, 1.0f);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
 			voices[i]->setOsc1Volume(value);
		}
	}

	void setOsc2Volume(float value)
	{
		value = audioUtils.getLogScaledVolume(value, 1.0f);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc2Volume(value);
		}
	}

	void setOsc3Volume(float value)
	{
		value = audioUtils.getLogScaledVolume(value, 1.0f);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc3Volume(value);
		}
	}

	void setOsc1Waveform(int value)
	{
		// Osc::Waveform waveform = getOsc1Waveform(audioUtils.calcComboBoxValue(value, OSC1WAVEFORM));
		Osc::Waveform waveform = getOsc1Waveform(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1Waveform(waveform);
		}
	}

	void setOsc2Waveform(int value)
	{
		// Osc::Waveform waveform = getOsc2Waveform(audioUtils.calcComboBoxValue(value, OSC2WAVEFORM));
		Osc::Waveform waveform = getOsc2Waveform(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc2Waveform(waveform);
		}
	}

	void setOsc1Tune(int value)
	{
		//value = audioUtils.getOscTuneValue(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1Tune(value);
		}
	}

	void setOsc2Tune(int value)
	{
		//value = audioUtils.getOscTuneValue(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc2Tune(value);
		}
	}

	void setOsc1FineTune(float value)
	{
		//value = audioUtils.getOscFineTuneValue(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1FineTune(value);
		}
	}

	void setOsc2FineTune(float value)
	{
		//value = audioUtils.getOscFineTuneValue(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc2FineTune(value);
		}
	}


	void setOscSync(bool value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOscSync(value);
		}
	}

	void setCutoff(float value)
	{
        this->cutoff = audioUtils.getLogScaledValueFilter(value);
	}

	void setCutoffFast(float value)
	{
		setCutoff(value);
		cutoffFiltered->currentValue = audioUtils.getLogScaledValueFilter(value);
	}

	void setResonance(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setResonance(value);
		}
	}

	void setKeyfollow(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setKeyfollow(value);
		}
	}

	void setFilterContour(float value)
	{
		value = audioUtils.getLogScaledValueCentered(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterContour(value);
		}
	}

	void setFilterAttack(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterAttack(value);
		}
	}

	void setFilterDecay(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterDecay(value);
		}
	}

	void setFilterSustain(float value)
	{
        value = audioUtils.getLogScaledVolume(value, 1.0f);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterSustain(value);
		}
	}

	void setFilterRelease(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterRelease(value);
		}
	}

	void setAmpAttack(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setAmpAttack(value);
		}
	}

	void setAmpDecay(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setAmpDecay(value);
		}
	}

	void setAmpSustain(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setAmpSustain(value);
		}
	}

	void setAmpRelease(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setAmpRelease(value);
		}
	}

	void setPortamentoMode(float value)
	{
        int intValue = audioUtils.calcComboBoxValue(value, PORTAMENTOMODE);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setPortamentoMode(intValue);
		}
	}

	void setPortamento(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setPortamento(value);
		}
	}

	void setOsc1Pw(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1Pw(value);
		}
	}

	void setOsc1Phase(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1Phase(value);
		}
	}

	void setOsc1Fm(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc1Fm(value);
		}
	}

	void setOsc2Phase(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOsc2Phase(value);
		}
	}

	void setLfo1Rate(float value, float bmp)
	{
		lfoHandler1->setRate(value, bmp);
	}

	void setLfo2Rate(float value, float bmp)
	{
		lfoHandler2->setRate(value, bmp);
	}

	void setLfo1Amount(float value)
	{
		lfoHandler1->setAmount(audioUtils.getLogScaledValueCentered(value));
	}

	void setLfo2Amount(float value)
	{
		lfoHandler2->setAmount(audioUtils.getLogScaledValueCentered(value));
	}

	void setFreeAdAttack(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFreeAdAttack(value);
		}
	}

	void setFreeAdDecay(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFreeAdDecay(value);
		}
	}

	void setFreeAdAmount(float value)
	{
        value = audioUtils.getLogScaledValueCentered(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFreeAdAmount(value);
		}
	}

	void setFreeAdDestination(int value)
	{
        //int intValue = audioUtils.calcComboBoxValue(value, FREEADDESTINATION);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFreeAdDestination(value);
		}
	}

	void setLfo1Sync(float value, float rate, float bpm)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setLfo1Sync(value, rate, bpm);
		}
	}

	void setLfo1KeyTrigger(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setLfo1KeyTrigger(value);
		}
	}

	void setLfo2Sync(float value, float rate, float bpm)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setLfo2Sync(value, rate, bpm);
		}
	}

	void setLfo2KeyTrigger(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setLfo2KeyTrigger(value);
		}
	}

	void setLfo1Phase(float value)
	{
        this->lfoHandler1->setPhase(value);
	}

	void setLfo2Phase(float value)
	{
        this->lfoHandler2->setPhase(value);
	}

	void setLfo1Waveform(float value)
	{
        this->lfoHandler1->setWaveform(value);
	}

	void setLfo2Waveform(float value)
	{
        this->lfoHandler2->setWaveform(value);
	}

	void setVelocityVolume(float value)
	{
        this->velocityHandler->setVolume(value);
	}

	void setVelocityContour(float value)
	{
        this->velocityHandler->setContour(value);
	}

	void setVelocityCutoff(float value)
	{
        this->velocityHandler->setCutoff(value);
	}

	void setPitchwheelCutoff(float value)
	{
        this->pitchwheelHandler->setCutoff(value);
	}

	void setPitchwheelPitch(float value)
	{
        this->pitchwheelHandler->setPitch(value);
	}

	void setHighPass(float value)
	{
        highPass->setCutoff(audioUtils.getLogScaledValue(value));
	}

	void setMastertune(float value)
	{
    value = (value + 1) * 0.5;
    value = audioUtils.getLogScaledLinearValueCentered(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setMastertune(value);
		}
	}

	void setTranspose(int value)
	{
    int intValue = value * 12;
    //int intValue = audioUtils.getTranspose(value);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setTranspose(intValue);
		}
	}

    void setRingmodulation(float value)
    {
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setRingmodulation(value);
		}
    }

    void setPitchwheelAmount(float value)
    {
        this->pitchwheelHandler->setAmount(value);
    }

	void setLfo1Destination(int value)
	{
		value = value + 1;

		// int intValue = audioUtils.calcComboBoxValue(value, LFO1DESTINATION);
		switch (value)
		{
		case 1:
			lfoHandler1->setDestination(LfoHandler1::NOTHING);
			break;
		case 2:
			lfoHandler1->setDestination(LfoHandler1::FILTER);
			break;
		case 3:
			lfoHandler1->setDestination(LfoHandler1::OSC1PITCH);
			break;
		case 4:
			lfoHandler1->setDestination(LfoHandler1::OSC2PITCH);
			break;
		case 5:
			lfoHandler1->setDestination(LfoHandler1::PW);
			break;
		case 6:
			lfoHandler1->setDestination(LfoHandler1::FM);
			break;
		case 7:
            lfoHandler1->setDestination(LfoHandler1::LFO2RATE);
			break;
		case 8:
            lfoHandler1->setDestination(LfoHandler1::OSC12PITCH);
			break;
		}
	}

	void setLfo2Destination(int value)
	{
		value = value + 1;
		// int intValue = audioUtils.calcComboBoxValue(value, LFO2DESTINATION);
		switch (value)
		{
		case 1:
			lfoHandler2->setDestination(LfoHandler2::NOTHING);
			break;
		case 2:
			lfoHandler2->setDestination(LfoHandler2::FILTER);
			break;
		case 3:
			lfoHandler2->setDestination(LfoHandler2::OSC1PITCH);
			break;
		case 4:
			lfoHandler2->setDestination(LfoHandler2::OSC2PITCH);
			break;
		case 5:
			lfoHandler2->setDestination(LfoHandler2::PAN);
			break;
		case 6:
			lfoHandler2->setDestination(LfoHandler2::VOLUME);
			break;
		case 7:
			lfoHandler2->setDestination(LfoHandler2::LFO1RATE);
			break;
		case 8:
      lfoHandler2->setDestination(LfoHandler2::OSC12PITCH);
			break;
		}
	}

	void setPanic()
	{
		voiceManager->reset();
	}

	void setDetune(float value)
	{
        value *= value;
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setDetune(value);
		}
	}

	void setVintageNoise(float value)
	{
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setVintageNoise(value);
		}
	}

	void setFiltertype(int value)
	{
        // int intValue = audioUtils.calcComboBoxValue(value, FILTERTYPE);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFiltertype(value);
		}
	}

    void setFilterDrive(float value)
    {
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setFilterDrive(value);
		}
    }

    void setOscBitcrusher(float value)
    {
    value = 1 + (value * -1);
		SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			voices[i]->setOscBitcrusher(value);
		}
    }

    void reset()
    {
        voiceManager->reset();
    }

    void triggerLfoToHost(const float bmp, const float samplePosition)
    {
        if (lfoHandler1->isSync || lfoHandler2->isSync)
		{
            if (lfoHandler1->isSync)
            {
			    float samplesPerBeat = 60.0f / bmp * sampleRate;
			    float lfoInc = lfoHandler1->getLfoInc() / 256.0f;
			    float phase = lfoInc * samplesPerBeat * samplePosition;
			    lfoHandler1->setHostPhase(phase - floorf(phase));
            }
            if (lfoHandler2->isSync)
            {
			    float samplesPerBeat = 60.0f / bmp * sampleRate;
			    float lfoInc = lfoHandler2->getLfoInc() / 256.0f;
			    float phase = lfoInc * samplesPerBeat * samplePosition;
			    lfoHandler2->setHostPhase(phase - floorf(phase));
            }
		}
    }

	void process(float *sampleL, float *sampleR)
	{
        float denormalNoise = this->denormalNoise->getNextSample() * 0.00000001f;
		*sampleL = denormalNoise;
		*sampleR = denormalNoise;

		// Parameter lp filtering
		float cutoff = cutoffFiltered->tick(this->cutoff);

		// Process voices
		bool playingNotes = false;
        SynthVoice** voices = voiceManager->getAllVoices();
		for (int i = 0; i < voiceManager->MAX_VOICES; i++)
		{
			playingNotes |= voices[i]->process(sampleL, sampleR, cutoff);
		}

        // highPass->tick(sampleL);
        *sampleR = *sampleL;

		if (playingNotes)
		{
		    // calc next lfo value with modulation of the other LFO
            lfoHandler1->setRateMultiplier(this->lfoHandler2->getLfo1());
		    lfoHandler1->process();
            lfoHandler2->setRateMultiplier(this->lfoHandler1->getLfo2());
		    lfoHandler2->process();

            // FIXME: loose stereo information here of a voice
            *sampleL *= this->lfoHandler2->getVolume();
            *sampleR = *sampleL;

            this->stereoPan->process(sampleL, sampleR);
		}

		*sampleL *= volume;
		*sampleR *= volume;
	}
};
#endif
