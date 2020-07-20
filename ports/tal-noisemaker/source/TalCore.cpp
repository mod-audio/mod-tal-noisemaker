/*
==============================================================================

This file is part of the JUCE library - "Jules' Utility Class Extensions"
Copyright 2004-7 by Raw Material Software ltd.

------------------------------------------------------------------------------

JUCE can be redistributed and/or modified under the terms of the
GNU General Public License, as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.

JUCE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JUCE; if not, visit www.gnu.org/licenses or write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA

------------------------------------------------------------------------------

If you'd like to release a closed-source product which uses JUCE, commercial
licenses are also available: visit www.rawmaterialsoftware.com/juce for
more information.

==============================================================================
*/


#include "includes.h"
#include "TalComponent.h"
#include "ProgramChunk.h"

/**
This function must be implemented to create a new instance of your
plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TalCore();
}

TalCore::TalCore()
{
    this->numPrograms = 256;
    this->currentNumberOfVoicesNormalized = -1.0f;

    // init engine
    if (this->getSampleRate() > 0)
        sampleRate = (float)this->getSampleRate();
    else
        sampleRate = 44100.0f;

    engine = new SynthEngine(sampleRate);

    talPresets = new TalPreset*[this->numPrograms];

    for (int i = 0; i < this->numPrograms; i++) talPresets[i] = new TalPreset();
    curProgram = 0;

    // load factory presets
    //ProgramChunk chunk;
    //setStateInformationString(chunk.getXmlChunk());

    setCurrentProgram(curProgram);

    nextMidiMessage = new MidiMessage(0xF0);
    midiMessage = new MidiMessage(0xF0);

    this->bpm = 120.0f;
}

TalCore::~TalCore()
{
    for (int i = 0; i < this->numPrograms; i++) delete talPresets[i];
    if (talPresets) delete[] talPresets;
    if (engine) delete engine;

    delete nextMidiMessage;
    delete midiMessage;
}

const String TalCore::getName() const
{
    return "Noize Mak3r";
}

int TalCore::getNumParameters()
{
    return (int)NUMPARAM;
}

float TalCore::getParameter (int index)
{
    if (index < NUMPARAM)
        return talPresets[curProgram]->programData[index];
    else
        return 0;
}

void TalCore::setParameter (int index, float newValue)
{
    if (index < NUMPARAM)
    {
        talPresets[curProgram]->programData[index] = newValue;

        switch(index)
        {
        case VOLUME:
            engine->setVolume(newValue);
            break;
        case CUTOFF:
            engine->setCutoff(newValue);
            break;
        case RESONANCE:
            engine->setResonance(newValue);
            break;
        case FILTERCONTOUR:
            engine->setFilterContour(newValue);
            break;
        case KEYFOLLOW:
            engine->setKeyfollow(newValue);
            break;
        case FILTERATTACK:
            engine->setFilterAttack(newValue);
            break;
        case FILTERDECAY:
            engine->setFilterDecay(newValue);
            break;
        case FILTERSUSTAIN:
            engine->setFilterSustain(newValue);
            break;
        case FILTERRELEASE:
            engine->setFilterRelease(newValue);
            break;
        case AMPATTACK:
            engine->setAmpAttack(newValue);
            break;
        case AMPDECAY:
            engine->setAmpDecay(newValue);
            break;
        case AMPSUSTAIN:
            engine->setAmpSustain(newValue);
            break;
        case AMPRELEASE:
            engine->setAmpRelease(newValue);
            break;
        case OSC1VOLUME:
            engine->setOsc1Volume(newValue);
            break;
        case OSC2VOLUME:
            engine->setOsc2Volume(newValue);
            break;
        case OSC3VOLUME:
            engine->setOsc3Volume(newValue);
            break;
        case OSC1WAVEFORM:
            engine->setOsc1Waveform(newValue);
            break;
        case OSC2WAVEFORM:
            engine->setOsc2Waveform(newValue);
            break;
        case OSC1TUNE:
            engine->setOsc1Tune(newValue);
            break;
        case OSC2TUNE:
            engine->setOsc2Tune(newValue);
            break;
        case OSC1FINETUNE:
            engine->setOsc1FineTune(newValue);
            break;
        case OSC2FINETUNE:
            engine->setOsc2FineTune(newValue);
            break;
        case OSCSYNC:
            engine->setOscSync(newValue > 0.0f);
            break;
        case PANIC:
            engine->setPanic();
            break;
        case PORTAMENTO:
            engine->setPortamento(newValue);
            break;
        case PORTAMENTOMODE:
            engine->setPortamentoMode(newValue);
            break;
        case LFO1RATE:
            engine->setLfo1Rate(newValue, this->bpm);
            break;
        case LFO2RATE:
            engine->setLfo2Rate(newValue, this->bpm);
            break;
        case LFO1AMOUNT:
            engine->setLfo1Amount(newValue);
            break;
        case LFO2AMOUNT:
            engine->setLfo2Amount(newValue);
            break;
        case LFO1WAVEFORM:
            engine->setLfo1Waveform(newValue);
            break;
        case LFO2WAVEFORM:
            engine->setLfo2Waveform(newValue);
            break;
        case LFO1DESTINATION:
            engine->setLfo1Destination(newValue);
            break;
        case LFO2DESTINATION:
            engine->setLfo2Destination(newValue);
            break;
        case OSC1PW:
            engine->setOsc1Pw(newValue);
            break;
        case OSC1PHASE:
            engine->setOsc1Phase(newValue);
            break;
        case OSC2FM:
            engine->setOsc1Fm(newValue);
            break;
        case OSC2PHASE:
            engine->setOsc2Phase(newValue);
            break;
        case FREEADATTACK:
            engine->setFreeAdAttack(newValue);
            break;
        case FREEADDECAY:
            engine->setFreeAdDecay(newValue);
            break;
        case FREEADAMOUNT:
            engine->setFreeAdAmount(newValue);
            break;
        case FREEADDESTINATION:
            engine->setFreeAdDestination(newValue);
            break;
        case LFO1SYNC:
            engine->setLfo1Sync(newValue, getParameter(LFO1RATE), this->bpm);
            break;
        case LFO1KEYTRIGGER:
            engine->setLfo1KeyTrigger(newValue);
            break;
        case LFO1PHASE:
            engine->setLfo1Phase(newValue);
            break;
        case LFO2SYNC:
            engine->setLfo2Sync(newValue, getParameter(LFO2RATE), this->bpm);
            break;
        case LFO2KEYTRIGGER:
            engine->setLfo2KeyTrigger(newValue);
            break;
        case LFO2PHASE:
            engine->setLfo2Phase(newValue);
            break;
        case VELOCITYVOLUME:
            engine->setVelocityVolume(newValue);
            break;
        case VELOCITYCONTOUR:
            engine->setVelocityContour(newValue);
            break;
        case VELOCITYCUTOFF:
            engine->setVelocityCutoff(newValue);
            break;
        case PITCHWHEELCUTOFF:
            engine->setPitchwheelCutoff(newValue);
            break;
        case PITCHWHEELPITCH:
            engine->setPitchwheelPitch(newValue);
            break;
        case OSCMASTERTUNE:
            engine->setMastertune(newValue);
            break;
        case OCTAVE:
            engine->setOctave(newValue);
            break;
        case RINGMODULATION:
            engine->setRingmodulation(newValue);
            break;
        case OSCBITCRUSHER:
            engine->setOscBitcrusher(newValue);
            break;
        case FILTERTYPE:
            engine->setFiltertype(newValue);
            break;
        }

        sendChangeMessage();
    }
}

const String TalCore::getParameterName (int index)
{
    switch(index)
    {
    case VOLUME: return "volume";
    case OSC1VOLUME: return "osc1volume";
    case OSC2VOLUME: return "osc2volume";
    case OSC3VOLUME: return "osc3volume";

    case OSC1WAVEFORM: return "osc1waveform";
    case OSC2WAVEFORM: return "osc2waveform";
    case OSCSYNC: return "oscsync";

    case OSCMASTERTUNE: return "oscmastertune";
    case OSC1TUNE: return "osc1tune";
    case OSC2TUNE: return "osc2tune";
    case OSC1FINETUNE: return "osc1finetune";
    case OSC2FINETUNE: return "osc2finetune";

    case FILTERTYPE: return "filtertype";
    case CUTOFF: return "cutoff";
    case RESONANCE: return "resonance";
    case KEYFOLLOW: return "keyfollow";
    case FILTERCONTOUR: return "filtercontour";
    case FILTERATTACK: return "filterattack";
    case FILTERDECAY: return "filterdecay";
    case FILTERSUSTAIN: return "filtersustain";
    case FILTERRELEASE: return "filterrelease";

    case AMPATTACK: return "ampattack";
    case AMPDECAY: return "ampdecay";
    case AMPSUSTAIN: return "ampsustain";
    case AMPRELEASE: return "amprelease";

    case VOICES: return "voices";
    case PORTAMENTO: return "portamento";
    case PORTAMENTOMODE: return "portamentomode";

    case LFO1WAVEFORM: return "lfo1waveform";
    case LFO2WAVEFORM: return "lfo2waveform";
    case LFO1RATE: return "lfo1rate";
    case LFO2RATE: return "lfo2rate";
    case LFO1AMOUNT: return "lfo1amount";
    case LFO2AMOUNT: return "lfo2amount";
    case LFO1DESTINATION: return "lfo1destination";
    case LFO2DESTINATION: return "lfo2destination";
    case LFO1PHASE: return "lfo1phase";
    case LFO2PHASE: return "lfo2phase";

    case OSC1PW: return "osc1pw";
    case OSC2FM: return "osc2fm";
    case OSC1PHASE: return "osc1phase";
    case OSC2PHASE: return "osc2phase";
    case OCTAVE: return "octave";

    case FREEADATTACK: return "freeadattack";
    case FREEADDECAY: return "freeaddecay";
    case FREEADAMOUNT: return "freeadamount";
    case FREEADDESTINATION: return "freeaddestination";

    case LFO1SYNC: return "lfo1sync";
    case LFO1KEYTRIGGER: return "lfo1keytrigger";
    case LFO2SYNC: return "lfo2sync";
    case LFO2KEYTRIGGER: return "lfo2keytrigger";

    case VELOCITYVOLUME: return "velocityvolume";
    case VELOCITYCONTOUR: return "velocitycontour";
    case VELOCITYCUTOFF: return "velocitycutoff";
    case PITCHWHEELCUTOFF: return "pitchwheelcutoff";
    case PITCHWHEELPITCH: return "pitchwheelpitch";
    case RINGMODULATION: return "ringmodulation";


    case OSCBITCRUSHER: return "oscbitcrusher";

    }
    return String();
}

const String TalCore::getParameterText (int index)
{
    if (index < NUMPARAM)
    {
        return String(talPresets[curProgram]->programData[index], 2);
    }
    return String();
}

const String TalCore::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String TalCore::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool TalCore::isInputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::acceptsMidi() const
{
    return true;
}

bool TalCore::producesMidi() const
{
    return false;
}

void TalCore::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void TalCore::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TalCore::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    // Handle sample rate changes
    if (this->sampleRate != this->getSampleRate())
    {
        this->sampleRate = (float)this->getSampleRate();
        this->engine->setSampleRate(this->sampleRate);
        this->setCurrentProgram(this->curProgram);
    }

    // Number of voices
    if (this->currentNumberOfVoicesNormalized != this->getParameter(VOICES))
    {
        this->currentNumberOfVoicesNormalized = this->getParameter(VOICES);
        engine->setNumberOfVoices(this->getParameter(VOICES));
        setCurrentProgram(this->curProgram);
    }

    // bpm
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition(pos) && pos.bpm > 0)
    {
        if (this->bpm != pos.bpm)
        {
            this->bpm = (float)pos.bpm;
            this->engine->setLfo1Sync(getParameter(LFO1SYNC), getParameter(LFO1RATE), this->bpm);
            this->engine->setLfo2Sync(getParameter(LFO2SYNC), getParameter(LFO2RATE), this->bpm);
        }

        this->engine->triggerLfoToHost((const float)this->bpm, (const float)pos.ppqPosition);
    }
    else
    {
        this->bpm = 120.0f;
    }

    const ScopedLock sl (this->getCallbackLock());

    // midi messages
    MidiBuffer::Iterator midiIterator(midiMessages);
    hasMidiMessage = midiIterator.getNextEvent(*nextMidiMessage, midiEventPos);

    int numberOfChannels = getTotalNumOutputChannels();
    if (numberOfChannels == 2)
    {
        float *samples0 = buffer.getWritePointer(0);
        float *samples1 = buffer.getWritePointer(1);

        int samplePos = 0;
        int numSamples = buffer.getNumSamples();
        while (numSamples > 0)
        {
            processMidiPerSample(&midiIterator, samplePos);
            engine->process(samples0++, samples1++);

            numSamples--;
            samplePos++;
        }
    }
}

float TalCore::getBpm()
{
    return this->bpm;
}

inline void TalCore::processMidiPerSample(MidiBuffer::Iterator *midiIterator, const int samplePos)
{
    // There can be more than one event at the same position
    while (this->getNextEvent(midiIterator, samplePos))
    {
        if (midiMessage->isAllNotesOff() || midiMessage->isAllSoundOff() || midiMessage->isMidiStop())
        {
            engine->reset();
        }
        else if (midiMessage->isController())
        {
        }
        else if (midiMessage->isNoteOn())
        {
            engine->setNoteOn(midiMessage->getNoteNumber(), midiMessage->getFloatVelocity());
        }
        else if (midiMessage->isNoteOff())
        {
            engine->setNoteOff(midiMessage->getNoteNumber());
        }
        else if (midiMessage->isPitchWheel())
        {
            // [0..16383] center = 8192;
            engine->setPitchwheelAmount((midiMessage->getPitchWheelValue() - 8192.0f) / (16383.0f * 0.5f));
        }
    }
}

inline bool TalCore::getNextEvent(MidiBuffer::Iterator *midiIterator, const int samplePos)
{
    if (hasMidiMessage && midiEventPos <= samplePos)
    {
        *midiMessage = *nextMidiMessage;
        hasMidiMessage = midiIterator->getNextEvent(*nextMidiMessage, midiEventPos);
        return true;
    }
    return false;
}

#if ! JUCE_AUDIOPROCESSOR_NO_GUI
AudioProcessorEditor* TalCore::createEditor()
{
    // return new TalComponent (this);
    return 0;
}
#endif

void TalCore::getStateInformation (MemoryBlock& destData)
{
    // header
    XmlElement tal("tal");
    tal.setAttribute ("curprogram", curProgram);
    tal.setAttribute ("version", 1.7);

    // programs
    XmlElement *programList = new XmlElement ("programs");
    for (int i = 0; i < this->numPrograms; i++)
    {
        getXmlPrograms(programList, i);
    }
    tal.addChildElement(programList);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (tal, destData);

    // use this for new factory presets
    //#ifdef _DEBUG && WIN32
    //File *file = new File("e:/presetsSave.txt");
    //String myXmlDoc = tal.createDocument ("presetsSave.txt");
    //file->replaceWithText(myXmlDoc);
    //#endif
}

void TalCore::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    // header
    XmlElement tal("tal");
    tal.setAttribute ("curprogram", curProgram);
    tal.setAttribute ("version", 1.7);

    // programs
    XmlElement *programList = new XmlElement ("programs");

    getXmlPrograms(programList, this->curProgram);
    tal.addChildElement(programList);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (tal, destData);
}

XmlElement* TalCore::getCurrentProgramStateInformationAsXml()
{
    MemoryBlock destData;
    this->getCurrentProgramStateInformation(destData);
    return this->getXmlFromBinary(destData.getData(), destData.getSize());
}

void TalCore::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    //XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);
    //setStateInformationFromXml(xmlState);
}

void TalCore::setStateInformation (const void* data, int sizeInBytes)
{
}

void TalCore::setStateInformationFromXml(XmlElement* xmlState)
{
}

void TalCore::getXmlPrograms(XmlElement *programList, int programNumber)
{
}

void TalCore::setXmlPrograms(XmlElement* e, int programNumber, float version)
{
}

void TalCore::setStateInformationString (const String& data)
{
}

int TalCore::getNumPrograms ()
{
    return this->numPrograms;
}

int TalCore::getCurrentProgram ()
{
    return curProgram;
}

//CHECK verschill als dit problemen geeft
void TalCore::setCurrentProgram (int index)
{
    if (index < this->numPrograms)
    {
        curProgram = index;

        for (int i = 0; i < NUMPARAM; i++)
        {
            setParameter(i, talPresets[curProgram]->programData[i]);
        }

        this->engine->reset();
        sendChangeMessage ();
    }
}

const String TalCore::getProgramName (int index)
{
    if (index < this->numPrograms)
        return talPresets[index]->name;
    else
        return "Invalid";
}

void TalCore::changeProgramName (int index, const String& newName)
{
    if (index < this->numPrograms)
        talPresets[index]->name = newName;
}
