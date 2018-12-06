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
            std::cout << " VOLUME = " << VOLUME << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case CUTOFF:
            engine->setCutoff(newValue);
            std::cout << " CUTOFF = " << CUTOFF << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case RESONANCE:
            engine->setResonance(newValue);
            std::cout << " RESONANCE = " << RESONANCE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FILTERCONTOUR:
            engine->setFilterContour(newValue);
            std::cout << " FILTERCONTOUR = " << FILTERCONTOUR << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case KEYFOLLOW:
            engine->setKeyfollow(newValue);
            std::cout << " KEYFOLLOW = " << KEYFOLLOW << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FILTERATTACK:
            engine->setFilterAttack(newValue);
            std::cout << " FILTERATTACK = " << FILTERATTACK << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FILTERDECAY:
            engine->setFilterDecay(newValue);
            std::cout << " FILTERDECAY = " << FILTERDECAY << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FILTERSUSTAIN:
            engine->setFilterSustain(newValue);
            std::cout << " FILTERSUSTAIN = " << FILTERSUSTAIN << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FILTERRELEASE:
            engine->setFilterRelease(newValue);
            std::cout << " FILTERRELEASE = " << FILTERRELEASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case AMPATTACK:
            engine->setAmpAttack(newValue);
            std::cout << " AMPATTACK = " << AMPATTACK << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case AMPDECAY:
            engine->setAmpDecay(newValue);
            std::cout << " AMPDECAY = " << AMPDECAY << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case AMPSUSTAIN:
            engine->setAmpSustain(newValue);
            std::cout << " AMPSUSTAIN = " << AMPSUSTAIN << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case AMPRELEASE:
            engine->setAmpRelease(newValue);
            std::cout << " AMPRELEASE = " << AMPRELEASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1VOLUME:
            engine->setOsc1Volume(newValue);
            std::cout << " OSC1VOLUME = " << OSC1VOLUME << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2VOLUME:
            engine->setOsc2Volume(newValue);
            std::cout << " OSC2VOLUME = " << OSC2VOLUME << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC3VOLUME:
            engine->setOsc3Volume(newValue);
            std::cout << " OSC3VOLUME = " << OSC3VOLUME << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1WAVEFORM:
            engine->setOsc1Waveform(newValue);
            std::cout << " OSC1WAVEFORM = " << OSC1WAVEFORM << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2WAVEFORM:
            engine->setOsc2Waveform(newValue);
            std::cout << " OSC2WAVEFORM = " << OSC2WAVEFORM << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1TUNE:
            engine->setOsc1Tune(newValue);
            std::cout << " OSC1TUNE = " << OSC1TUNE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2TUNE:
            engine->setOsc2Tune(newValue);
            std::cout << " OSC2TUNE = " << OSC2TUNE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1FINETUNE:
            engine->setOsc1FineTune(newValue);
            std::cout << " OSC1FINETUNE = " << OSC1FINETUNE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2FINETUNE:
            engine->setOsc2FineTune(newValue);
            std::cout << " OSC2FINETUNE = " << OSC2FINETUNE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSCSYNC:
            engine->setOscSync(newValue > 0.0f);
            std::cout << " OSCSYNC = " << OSCSYNC << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case PANIC:
            engine->setPanic();
            std::cout << " PANIC = " << PANIC << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case PORTAMENTO:
            engine->setPortamento(newValue);
            std::cout << " PORTAMENTO = " << PORTAMENTO << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case PORTAMENTOMODE:
            engine->setPortamentoMode(newValue);
            std::cout << " PORTAMENTOMODE = " << PORTAMENTOMODE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1RATE:
            engine->setLfo1Rate(newValue, this->bpm);
            std::cout << " LFO1RATE = " << LFO1RATE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2RATE:
            engine->setLfo2Rate(newValue, this->bpm);
            std::cout << " LFO2RATE = " << LFO2RATE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1AMOUNT:
            engine->setLfo1Amount(newValue);
            std::cout << " LFO1AMOUNT = " << LFO1AMOUNT << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2AMOUNT:
            engine->setLfo2Amount(newValue);
            std::cout << " LFO2AMOUNT = " << LFO2AMOUNT << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1WAVEFORM:
            engine->setLfo1Waveform(newValue);
            std::cout << " LFO1WAVEFORM = " << LFO1WAVEFORM << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2WAVEFORM:
            engine->setLfo2Waveform(newValue);
            std::cout << " LFO2WAVEFORM = " << LFO2WAVEFORM << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1DESTINATION:
            engine->setLfo1Destination(newValue);
            std::cout << " LFO1DESTINATION = " << LFO1DESTINATION << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2DESTINATION:
            engine->setLfo2Destination(newValue);
            std::cout << " LFO2DESTINATION = " << LFO2DESTINATION << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1PW:
            engine->setOsc1Pw(newValue);
            std::cout << " OSC1PW = " << OSC1PW << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC1PHASE:
            engine->setOsc1Phase(newValue);
            std::cout << " OSC1PHASE = " << OSC1PHASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2FM:
            engine->setOsc1Fm(newValue);
            std::cout << " OSC2FM = " << OSC2FM << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSC2PHASE:
            engine->setOsc2Phase(newValue);
            std::cout << " OSC2PHASE = " << OSC2PHASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FREEADATTACK:
            engine->setFreeAdAttack(newValue);
            std::cout << " FREEADATTACK = " << FREEADATTACK << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FREEADDECAY:
            engine->setFreeAdDecay(newValue);
            std::cout << " FREEADDECAY = " << FREEADDECAY << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FREEADAMOUNT:
            engine->setFreeAdAmount(newValue);
            std::cout << " FREEADAMOUNT = " << FREEADAMOUNT << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case FREEADDESTINATION:
            engine->setFreeAdDestination(newValue);
            std::cout << " FREEADDESTINATION = " << FREEADDESTINATION << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1SYNC:
            engine->setLfo1Sync(newValue, getParameter(LFO1RATE), this->bpm);
            std::cout << " LFO1SYNC = " << LFO1SYNC << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1KEYTRIGGER:
            engine->setLfo1KeyTrigger(newValue);
            std::cout << " LFO1KEYTRIGGER = " << LFO1KEYTRIGGER << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO1PHASE:
            engine->setLfo1Phase(newValue);
            std::cout << " LFO1PHASE = " << LFO1PHASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2SYNC:
            engine->setLfo2Sync(newValue, getParameter(LFO2RATE), this->bpm);
            std::cout << " LFO2SYNC = " << LFO2SYNC << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2KEYTRIGGER:
            engine->setLfo2KeyTrigger(newValue);
            std::cout << " LFO2KEYTRIGGER = " << LFO2KEYTRIGGER << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case LFO2PHASE:
            engine->setLfo2Phase(newValue);
            std::cout << " LFO2PHASE = " << LFO2PHASE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case VELOCITYVOLUME:
            engine->setVelocityVolume(newValue);
            std::cout << " VELOCITYVOLUME = " << VELOCITYVOLUME << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case VELOCITYCONTOUR:
            engine->setVelocityContour(newValue);
            std::cout << " VELOCITYCONTOUR = " << VELOCITYCONTOUR << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case VELOCITYCUTOFF:
            engine->setVelocityCutoff(newValue);
            std::cout << " VELOCITYCUTOFF = " << VELOCITYCUTOFF << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case PITCHWHEELCUTOFF:
            engine->setPitchwheelCutoff(newValue);
            std::cout << " PITCHWHEELCUTOFF = " << PITCHWHEELCUTOFF << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case PITCHWHEELPITCH:
            engine->setPitchwheelPitch(newValue);
            std::cout << " PITCHWHEELPITCH = " << PITCHWHEELPITCH << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSCMASTERTUNE:
            engine->setMastertune(newValue);
            std::cout << " OSCMASTERTUNE = " << OSCMASTERTUNE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OCTAVE:
            engine->setOctave(newValue);
            std::cout << " OCTAVE = " << OCTAVE << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case RINGMODULATION:
            engine->setRingmodulation(newValue);
            std::cout << " RINGMODULATION = " << RINGMODULATION << " index = "<< index << " value = " << newValue << std::endl;
            break;
        case OSCBITCRUSHER:
            engine->setOscBitcrusher(newValue);
            std::cout << " OSCBITCRUSHER = " <<  OSCBITCRUSHER << " index = " << index << " value = " << newValue << std::endl;
            break;
        case FILTERTYPE:
            engine->setFiltertype(newValue);
            std::cout << " FILTERTYPE = " << FILTERTYPE  << " index = "<< index << " value = " << newValue << std::endl;
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
    // use this helper function to get the XML from this binary blob..
    //XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

    //File *file = new File("e:/presetsLoad.txt");
    //xmlState->writeToFile(*file, T("tal"));

    //setStateInformationFromXml(xmlState);
}

void TalCore::setStateInformationFromXml(XmlElement* xmlState)
{
   // if (xmlState != 0 && xmlState->hasTagName("tal"))
   // {
   //     float version = (float)xmlState->getDoubleAttribute ("version", 1);

   //     XmlElement* programs = xmlState->getFirstChildElement();
   //     if (programs->hasTagName("programs"))
   //     {
   //         int programNumber = 0;

   //         if (programs->getNumChildElements() != 1)
   //         {
   //             curProgram = (int)xmlState->getIntAttribute ("curprogram", 1);

   //             forEachXmlChildElement (*programs, e)
   //             {
   //                 this->setXmlPrograms(e, programNumber, version);
   //                 programNumber++;
   //             }
   //         }
   //         else
   //         {
   //             this->setXmlPrograms(programs->getFirstChildElement(), curProgram, version);
   //         }
   //     }

   //     delete xmlState;
   //     this->setCurrentProgram(curProgram);
   //     this->sendChangeMessage ();
   // }
}

void TalCore::getXmlPrograms(XmlElement *programList, int programNumber)
{
    //    XmlElement* program = new XmlElement ("program");
    //    program->setAttribute ("programname", talPresets[programNumber]->name);
    //    program->setAttribute ("volume", talPresets[programNumber]->programData[VOLUME]);

    //    program->setAttribute ("filtertype", talPresets[programNumber]->programData[FILTERTYPE]);
    //    program->setAttribute ("cutoff", talPresets[programNumber]->programData[CUTOFF]);
    //    program->setAttribute ("resonance", talPresets[programNumber]->programData[RESONANCE]);
    //    program->setAttribute ("osc1volume", talPresets[programNumber]->programData[OSC1VOLUME]);
    //    program->setAttribute ("osc2volume", talPresets[programNumber]->programData[OSC2VOLUME]);
    //    program->setAttribute ("osc3volume", talPresets[programNumber]->programData[OSC3VOLUME]);

    //    program->setAttribute ("osc1waveform", talPresets[programNumber]->programData[OSC1WAVEFORM]);
    //    program->setAttribute ("osc2waveform", talPresets[programNumber]->programData[OSC2WAVEFORM]);

    //    program->setAttribute ("oscsync", talPresets[programNumber]->programData[OSCSYNC]);

    //    program->setAttribute ("oscmastertune", talPresets[programNumber]->programData[OSCMASTERTUNE]);
    //    program->setAttribute ("osc1tune", talPresets[programNumber]->programData[OSC1TUNE]);
    //    program->setAttribute ("osc2tune", talPresets[programNumber]->programData[OSC2TUNE]);
    //    program->setAttribute ("osc1finetune", talPresets[programNumber]->programData[OSC1FINETUNE]);
    //    program->setAttribute ("osc2finetune", talPresets[programNumber]->programData[OSC2FINETUNE]);
    //    program->setAttribute ("portamento", talPresets[programNumber]->programData[PORTAMENTO]);

    //    program->setAttribute ("keyfollow", talPresets[programNumber]->programData[KEYFOLLOW]);
    //    program->setAttribute ("filtercontour", talPresets[programNumber]->programData[FILTERCONTOUR]);
    //    program->setAttribute ("filterattack", talPresets[programNumber]->programData[FILTERATTACK]);
    //    program->setAttribute ("filterdecay", talPresets[programNumber]->programData[FILTERDECAY]);
    //    program->setAttribute ("filtersustain", talPresets[programNumber]->programData[FILTERSUSTAIN]);
    //    program->setAttribute ("filterrelease", talPresets[programNumber]->programData[FILTERRELEASE]);

    //    program->setAttribute ("ampattack", talPresets[programNumber]->programData[AMPATTACK]);
    //    program->setAttribute ("ampdecay", talPresets[programNumber]->programData[AMPDECAY]);
    //    program->setAttribute ("ampsustain", talPresets[programNumber]->programData[AMPSUSTAIN]);
    //    program->setAttribute ("amprelease", talPresets[programNumber]->programData[AMPRELEASE]);

    //    program->setAttribute ("voices", talPresets[programNumber]->programData[VOICES]);
    //    program->setAttribute ("portamentomode", talPresets[programNumber]->programData[PORTAMENTOMODE]);

    //    program->setAttribute ("lfo1waveform", talPresets[programNumber]->programData[LFO1WAVEFORM]);
    //    program->setAttribute ("lfo2waveform", talPresets[programNumber]->programData[LFO2WAVEFORM]);
    //    program->setAttribute ("lfo1rate", talPresets[programNumber]->programData[LFO1RATE]);
    //    program->setAttribute ("lfo2rate", talPresets[programNumber]->programData[LFO2RATE]);
    //    program->setAttribute ("lfo1amount", talPresets[programNumber]->programData[LFO1AMOUNT]);
    //    program->setAttribute ("lfo2amount", talPresets[programNumber]->programData[LFO2AMOUNT]);
    //    program->setAttribute ("lfo1destination", talPresets[programNumber]->programData[LFO1DESTINATION]);
    //    program->setAttribute ("lfo2destination", talPresets[programNumber]->programData[LFO2DESTINATION]);
    //    program->setAttribute ("lfo1phase", talPresets[programNumber]->programData[LFO1PHASE]);
    //    program->setAttribute ("lfo2phase", talPresets[programNumber]->programData[LFO2PHASE]);

    //    program->setAttribute ("osc1pw", talPresets[programNumber]->programData[OSC1PW]);
    //    program->setAttribute ("osc2fm", talPresets[programNumber]->programData[OSC2FM]);
    //    program->setAttribute ("osc1phase", talPresets[programNumber]->programData[OSC1PHASE]);
    //    program->setAttribute ("osc2phase", talPresets[programNumber]->programData[OSC2PHASE]);
    //    program->setAttribute ("octave", talPresets[programNumber]->programData[OCTAVE]);

    //    program->setAttribute ("freeadattack", talPresets[programNumber]->programData[FREEADATTACK]);
    //    program->setAttribute ("freeaddecay", talPresets[programNumber]->programData[FREEADDECAY]);
    //    program->setAttribute ("freeadamount", talPresets[programNumber]->programData[FREEADAMOUNT]);
    //    program->setAttribute ("freeaddestination", talPresets[programNumber]->programData[FREEADDESTINATION]);

    //    program->setAttribute ("lfo1sync", talPresets[programNumber]->programData[LFO1SYNC]);
    //    program->setAttribute ("lfo1keytrigger", talPresets[programNumber]->programData[LFO1KEYTRIGGER]);
    //    program->setAttribute ("lfo2sync", talPresets[programNumber]->programData[LFO2SYNC]);
    //    program->setAttribute ("lfo2keytrigger", talPresets[programNumber]->programData[LFO2KEYTRIGGER]);

    //    program->setAttribute ("velocityvolume", talPresets[programNumber]->programData[VELOCITYVOLUME]);
    //    program->setAttribute ("velocitycontour", talPresets[programNumber]->programData[VELOCITYCONTOUR]);
    //    program->setAttribute ("velocitycutoff", talPresets[programNumber]->programData[VELOCITYCUTOFF]);
    //    program->setAttribute ("pitchwheelcutoff", talPresets[programNumber]->programData[PITCHWHEELCUTOFF]);
    //    program->setAttribute ("pitchwheelpitch", talPresets[programNumber]->programData[PITCHWHEELPITCH]);
    //    program->setAttribute ("ringmodulation", talPresets[programNumber]->programData[RINGMODULATION]);

    //    programList->addChildElement(program);
}

void TalCore::setXmlPrograms(XmlElement* e, int programNumber, float version)
{
  //  if (e->hasTagName("program") && programNumber < this->numPrograms)
  //  {
  //      talPresets[programNumber]->name = e->getStringAttribute ("programname", "Not Saved" + String(programNumber));
  //      talPresets[programNumber]->programData[VOLUME] = (float) e->getDoubleAttribute ("volume", 0.5f);

  //      talPresets[programNumber]->programData[FILTERTYPE] = (float) e->getDoubleAttribute ("filtertype", 1.0f);
  //      talPresets[programNumber]->programData[CUTOFF] = (float) e->getDoubleAttribute ("cutoff", 1.0f);
  //      talPresets[programNumber]->programData[RESONANCE] = (float) e->getDoubleAttribute ("resonance", 0.0f);
  //      talPresets[programNumber]->programData[OSC1VOLUME] = (float) e->getDoubleAttribute ("osc1volume", 0.8f);
  //      talPresets[programNumber]->programData[OSC2VOLUME] = (float) e->getDoubleAttribute ("osc2volume", 0.0f);
  //      talPresets[programNumber]->programData[OSC3VOLUME] = (float) e->getDoubleAttribute ("osc3volume", 0.8f);

  //      talPresets[programNumber]->programData[OSC1WAVEFORM] = (float) e->getDoubleAttribute ("osc1waveform", 1.0f);
  //      talPresets[programNumber]->programData[OSC2WAVEFORM] = (float) e->getDoubleAttribute ("osc2waveform", 1.0f);

  //      talPresets[programNumber]->programData[OSCSYNC] = (float) e->getDoubleAttribute ("oscsync", 0.0f);

  //      talPresets[programNumber]->programData[OSCMASTERTUNE] = (float) e->getDoubleAttribute ("oscmastertune", 0.5f);
  //      talPresets[programNumber]->programData[OSC1TUNE] = (float) e->getDoubleAttribute ("osc1tune", 0.25f);
  //      talPresets[programNumber]->programData[OSC2TUNE] = (float) e->getDoubleAttribute ("osc2tune", 0.5f);
  //      talPresets[programNumber]->programData[OSC1FINETUNE] = (float) e->getDoubleAttribute ("osc1finetune", 0.5f);
  //      talPresets[programNumber]->programData[OSC2FINETUNE] = (float) e->getDoubleAttribute ("osc2finetune", 0.5f);

  //      talPresets[programNumber]->programData[KEYFOLLOW] = (float) e->getDoubleAttribute ("keyfollow", 0.0f);
  //      talPresets[programNumber]->programData[FILTERCONTOUR] = (float) e->getDoubleAttribute ("filtercontour", 0.5f);
  //      talPresets[programNumber]->programData[FILTERATTACK] = (float) e->getDoubleAttribute ("filterattack", 0.0f);
  //      talPresets[programNumber]->programData[FILTERDECAY] = (float) e->getDoubleAttribute ("filterdecay", 0.0f);
  //      talPresets[programNumber]->programData[FILTERSUSTAIN] = (float) e->getDoubleAttribute ("filtersustain", 1.0f);
  //      talPresets[programNumber]->programData[FILTERRELEASE] = (float) e->getDoubleAttribute ("filterrelease", 0.0f);

  //      talPresets[programNumber]->programData[AMPATTACK] = (float) e->getDoubleAttribute ("ampattack", 0.0f);
  //      talPresets[programNumber]->programData[AMPDECAY] = (float) e->getDoubleAttribute ("ampdecay", 0.0f);
  //      talPresets[programNumber]->programData[AMPSUSTAIN] = (float) e->getDoubleAttribute ("ampsustain", 1.0f);
  //      talPresets[programNumber]->programData[AMPRELEASE] = (float) e->getDoubleAttribute ("amprelease", 0.0f);

  //      talPresets[programNumber]->programData[VOICES] = (float) e->getDoubleAttribute ("voices", 1.0f);
  //      talPresets[programNumber]->programData[PORTAMENTOMODE] = (float) e->getDoubleAttribute ("portamentomode", 1.0f);
  //      talPresets[programNumber]->programData[PORTAMENTO] = (float) e->getDoubleAttribute ("portamento", 0.0f);

  //      talPresets[programNumber]->programData[LFO1WAVEFORM] = (float) e->getDoubleAttribute ("lfo1waveform", 0.0f);
  //      talPresets[programNumber]->programData[LFO2WAVEFORM] = (float) e->getDoubleAttribute ("lfo2waveform", 0.0f);
  //      talPresets[programNumber]->programData[LFO1RATE] = (float) e->getDoubleAttribute ("lfo1rate", 0.0f);
  //      talPresets[programNumber]->programData[LFO2RATE] = (float) e->getDoubleAttribute ("lfo2rate", 0.0f);
  //      talPresets[programNumber]->programData[LFO1AMOUNT] = (float) e->getDoubleAttribute ("lfo1amount", 0.5f);
  //      talPresets[programNumber]->programData[LFO2AMOUNT] = (float) e->getDoubleAttribute ("lfo2amount", 0.5f);
  //      talPresets[programNumber]->programData[LFO1DESTINATION] = (float) e->getDoubleAttribute ("lfo1destination", 1.0f);
  //      talPresets[programNumber]->programData[LFO2DESTINATION] = (float) e->getDoubleAttribute ("lfo2destination", 1.0f);
  //      talPresets[programNumber]->programData[LFO1PHASE] = (float) e->getDoubleAttribute ("lfo1phase", 0.0f);
  //      talPresets[programNumber]->programData[LFO2PHASE] = (float) e->getDoubleAttribute ("lfo2phase", 0.0f);

  //      talPresets[programNumber]->programData[OSC1PW] = (float) e->getDoubleAttribute ("osc1pw", 0.5f);
  //      talPresets[programNumber]->programData[OSC2FM] = (float) e->getDoubleAttribute ("osc2fm", 0.0f);
  //      talPresets[programNumber]->programData[OSC1PHASE] = (float) e->getDoubleAttribute ("osc1phase", 0.5f);
  //      talPresets[programNumber]->programData[OSC2PHASE] = (float) e->getDoubleAttribute ("osc2phase", 0.0f);
  //      talPresets[programNumber]->programData[OCTAVE] = (float) e->getDoubleAttribute ("octave", 0.5f);

  //      talPresets[programNumber]->programData[FREEADATTACK] = (float) e->getDoubleAttribute ("freeadattack", 0.0f);
  //      talPresets[programNumber]->programData[FREEADDECAY] = (float) e->getDoubleAttribute ("freeaddecay", 0.0f);
  //      talPresets[programNumber]->programData[FREEADAMOUNT] = (float) e->getDoubleAttribute ("freeadamount", 0.0f);
  //      talPresets[programNumber]->programData[FREEADDESTINATION] = (float) e->getDoubleAttribute ("freeaddestination", 1.0f);

  //      talPresets[programNumber]->programData[LFO1SYNC] = (float) e->getDoubleAttribute ("lfo1sync", 0.0f);
  //      talPresets[programNumber]->programData[LFO1KEYTRIGGER] = (float) e->getDoubleAttribute ("lfo1keytrigger", 0.0f);
  //      talPresets[programNumber]->programData[LFO2SYNC] = (float) e->getDoubleAttribute ("lfo2sync", 0.0f);
  //      talPresets[programNumber]->programData[LFO2KEYTRIGGER] = (float) e->getDoubleAttribute ("lfo2keytrigger", 0.0f);

  //      talPresets[programNumber]->programData[VELOCITYVOLUME] = (float) e->getDoubleAttribute ("velocityvolume", 0.0f);
  //      talPresets[programNumber]->programData[VELOCITYCONTOUR] = (float) e->getDoubleAttribute ("velocitycontour", 0.0f);
  //      talPresets[programNumber]->programData[VELOCITYCUTOFF] = (float) e->getDoubleAttribute ("velocitycutoff", 0.0f);
  //      talPresets[programNumber]->programData[PITCHWHEELCUTOFF] = (float) e->getDoubleAttribute ("pitchwheelcutoff", 0.0f);
  //      talPresets[programNumber]->programData[PITCHWHEELPITCH] = (float) e->getDoubleAttribute ("pitchwheelpitch", 0.0f);

  //      talPresets[programNumber]->programData[RINGMODULATION] = (float) e->getDoubleAttribute ("ringmodulation", 0.0f);

  //      talPresets[programNumber]->programData[OSCBITCRUSHER] = (float) e->getDoubleAttribute ("oscbitcrusher", 1.0f);


  //      // Preset compatibility
  //      if (version < 1.1f)
  //      {
  //          // make it compatibel to old version
  //          float osc1Waveform = talPresets[programNumber]->programData[OSC1WAVEFORM];
	//	    float valueSize = 1.0f / 2.0f - 0.001f;
	//	    if (osc1Waveform < valueSize)
	//	    {
	//		    talPresets[programNumber]->programData[OSC1WAVEFORM] = 1;
	//	    }
	//	    else if (osc1Waveform < valueSize * 2.0f)
	//	    {
	//		    talPresets[programNumber]->programData[OSC1WAVEFORM] = 2;
	//	    }
	//	    else
	//	    {
	//		    talPresets[programNumber]->programData[OSC1WAVEFORM] = 3;
	//	    }

  //          float osc2Waveform = talPresets[programNumber]->programData[OSC2WAVEFORM];
	//	    valueSize = 1.0f / 3.0f - 0.001f;
	//	    if (osc2Waveform < valueSize)
	//	    {
	//		    talPresets[programNumber]->programData[OSC2WAVEFORM] = 1;
	//	    }
	//	    else if (osc2Waveform < valueSize * 2.0f)
	//	    {
	//		    talPresets[programNumber]->programData[OSC2WAVEFORM] = 2;
	//	    }
	//	    else if (osc2Waveform < valueSize * 3.0f)
	//	    {
	//		    talPresets[programNumber]->programData[OSC2WAVEFORM] = 3;
	//	    }
	//	    else
	//	    {
	//		    talPresets[programNumber]->programData[OSC2WAVEFORM] = 4;
	//	    }
  //      }

  //      if (version < 1.6f)
  //      {
  //          talPresets[programNumber]->programData[VOICES] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[VOICES], VOICES);
  //          talPresets[programNumber]->programData[PORTAMENTOMODE] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[PORTAMENTOMODE], PORTAMENTOMODE);
  //          talPresets[programNumber]->programData[LFO1DESTINATION] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[LFO1DESTINATION], LFO1DESTINATION);
  //          talPresets[programNumber]->programData[LFO2DESTINATION] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[LFO2DESTINATION], LFO2DESTINATION);
  //          talPresets[programNumber]->programData[FREEADDESTINATION] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[FREEADDESTINATION], FREEADDESTINATION);
  //          talPresets[programNumber]->programData[FILTERTYPE] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[FILTERTYPE], FILTERTYPE);
  //          talPresets[programNumber]->programData[OSC1WAVEFORM] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[OSC1WAVEFORM], OSC1WAVEFORM);
  //          talPresets[programNumber]->programData[OSC2WAVEFORM] = audioUtils.calcComboBoxValueNormalized(talPresets[programNumber]->programData[OSC2WAVEFORM], OSC2WAVEFORM);
  //      }

  //      if (version < 1.7f)
  //      {
  //          // 2 new filter types
  //          int filtertypeOld = (int)floorf(talPresets[programNumber]->programData[FILTERTYPE] * (10 - 1.0f) + 1.0f + 0.5f);
  //          talPresets[programNumber]->programData[FILTERTYPE] = audioUtils.calcComboBoxValueNormalized(filtertypeOld, FILTERTYPE);
  //      }
  //  }
}

void TalCore::setStateInformationString (const String& data)
{
  //  XmlElement* const xmlState = XmlDocument::parse(data);
  //  setStateInformationFromXml(xmlState);
}

//String TalCore::getStateInformationString ()
//{
//    // header
//    XmlElement tal("tal");
//    tal.setAttribute ("curprogram", curProgram);
//    tal.setAttribute ("version", 1.7);
//
//    // programs
//    XmlElement *programList = new XmlElement ("programs");
//
//    getXmlPrograms(programList, this->curProgram);
//    tal.addChildElement(programList);
//
//    return tal.createDocument (String());
//}

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
