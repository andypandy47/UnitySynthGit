/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synthesizer_v1AudioProcessor::Synthesizer_v1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: MyAudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	attackTime(0.1f),
	decayTime(0.8f),
	sustainLevel(0.1f),
	releaseTime(0.1f)
#endif
{
	addParameter(triangleOnOff = new AudioParameterFloat("triangleOnOff", "TriangleOnOff", NormalisableRange<float>(0.0f, 1.0f), 1.0f));
	oscStateVals_01.add(triangleOnOff);
	addParameter(squareOnOff = new AudioParameterFloat("squareOnOff", "SquareOnOff", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
	oscStateVals_01.add(squareOnOff);
	addParameter(subOnOff = new AudioParameterFloat("subOnOff", "SubOnOff", NormalisableRange<float>(0.0f, 1.0f), 1.0f));
	oscStateVals_01.add(subOnOff);

	addParameter(filterTypeValue = new AudioParameterFloat("filterTypeValue", "FilterTypeValue", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
	filterTypeValue->addListener(this);
	addParameter(filterCuttoffAmount = new AudioParameterFloat("filterCutoffAmount", "FilterCutoffAmount", NormalisableRange<float>(0.0f, 1.0f), 0.05f));
	filterCuttoffAmount->addListener(this);
	addParameter(filterResAmount = new AudioParameterFloat("filterResAmount", "FilterResAmount", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	filterResAmount->addListener(this);

	addParameter(attackAmount = new AudioParameterFloat("attackAmout", "AttackAmout", NormalisableRange<float>(0.0f, 1.0f), 0.1f));
	attackAmount->addListener(this);
	addParameter(decayAmount = new AudioParameterFloat("decayAmount", "DecayAmount", NormalisableRange<float>(0.0f, 1.0f), 0.1f));
	decayAmount->addListener(this);
	addParameter(sustainAmount = new AudioParameterFloat("sustainAmount", "SustainAmount", NormalisableRange<float>(0.0f, 1.0f), 0.1f));
	sustainAmount->addListener(this);
	addParameter(releaseAmount = new AudioParameterFloat("releaseAmount", "ReleaseAmount", NormalisableRange<float>(0.0f, 1.0f), 0.1f));
	releaseAmount->addListener(this);

	addParameter(masterVolume = new AudioParameterFloat("masterVolume", "MasterVolume", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	
	m_MySynth_01.clearVoices();

	for (int i = 0; i < Voice_Count; i++)
	{
		m_MySynth_01.addVoice(new SynthVoice());
	}

	m_MySynth_01.clearSounds();
	m_MySynth_01.addSound(new SynthSound());
	m_MySynth_01.setNoteStealingEnabled(true);

	for (int i = 0; i < m_MySynth_01.getNumVoices(); i++)
	{
		if (m_MyVoice_01 = dynamic_cast<SynthVoice*>(m_MySynth_01.getVoice(i)))
		{
			m_MyVoice_01->InitVoice();
		}
	}

	Logger::getCurrentLogger()->writeToLog("Construct");

}

Synthesizer_v1AudioProcessor::~Synthesizer_v1AudioProcessor()
{
	Logger::getCurrentLogger()->writeToLog("Deconstruct");
}

//==============================================================================
const String Synthesizer_v1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Synthesizer_v1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Synthesizer_v1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Synthesizer_v1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Synthesizer_v1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Synthesizer_v1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Synthesizer_v1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Synthesizer_v1AudioProcessor::setCurrentProgram (int index)
{
}

const String Synthesizer_v1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Synthesizer_v1AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Synthesizer_v1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ignoreUnused(samplesPerBlock);
	lastSampleRate = sampleRate;
	m_MySynth_01.setCurrentPlaybackSampleRate(lastSampleRate);

	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	stateVariableFilter.reset();
	stateVariableFilter.prepare(spec);
	updateFilter();
}

void Synthesizer_v1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	m_MySynth_01.clearSounds();
	Logger::getCurrentLogger()->writeToLog("Releasing resources");
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Synthesizer_v1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Synthesizer_v1AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	float level = masterVolume->get();

	//Handle oscillator values, oscStates is checked in SynthVoice.h
	for (int i = 0; i < 3; i++)
	{
		if (oscStateVals_01[i]->get() == 1)
			oscStates[i] = true;
		else
			oscStates[i] = false;
	}

	//Loop through each voice
	for (int i = 0; i < m_MySynth_01.getNumVoices(); i++)
	{
		if (m_MyVoice_01 = dynamic_cast<SynthVoice*>(m_MySynth_01.getVoice(i)))
		{
			m_MyVoice_01->setEnvelopeValues(attackTime, decayTime, sustainLevel, releaseTime);
			m_MyVoice_01->setMasterVolume(level);
			m_MyVoice_01->setOscStates(oscStates[0], oscStates[1], oscStates[2]);
		}
	}
	buffer.clear();

	m_MySynth_01.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	updateFilter();
	dsp::AudioBlock<float> block(buffer);
	stateVariableFilter.process(dsp::ProcessContextReplacing<float> (block));
} 

//==============================================================================
bool Synthesizer_v1AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Synthesizer_v1AudioProcessor::createEditor()
{
    return new Synthesizer_v1AudioProcessorEditor (*this);
}

//==============================================================================
void Synthesizer_v1AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Synthesizer_v1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Synthesizer_v1AudioProcessor();
}

//==============================================================================
void Synthesizer_v1AudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
	if (parameterIndex == 3) //parameter is filter type
	{
		float paramValue = filterTypeValue->get();

		if (paramValue <= 0.3f)
			filterType = 0;
		else if (paramValue <= 0.6f)
			filterType = 1;
		else
			filterType = 2;
	}
	
	if (parameterIndex == 4) //parameter is cutoff
	{
		cutoffVal = filterCuttoffAmount->get() * 20000.0f;
		if (cutoffVal < 1)
			cutoffVal = 1;
	}

	if (parameterIndex == 5) //parameter is resonance
	{
		resVal = filterResAmount->get() * 10;
		if (resVal < 1)
			resVal = 1;
	}

	if (parameterIndex == 6) //parameter is attack
	{
		attackTime = attackAmount->get() * 10;
	}

	if (parameterIndex == 7) //parameter is decay
	{
		decayTime = decayAmount->get() * 10;
	}

	if (parameterIndex == 8) //parameter is sustain
	{
		sustainLevel = sustainAmount->get();
	}

	if (parameterIndex == 9) //parameter is release
	{
		releaseTime = releaseAmount->get() * 10;
	}
	
}

void Synthesizer_v1AudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{

}

void Synthesizer_v1AudioProcessor::updateFilter()
{
	switch (filterType)
	{
	case 0:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		break;
	case 1:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		break;
	case 2:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
		break;
	default:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		break;
	}
	stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoffVal, resVal);
}

void Synthesizer_v1AudioProcessor::midiNoteOn(int noteValue, int velocity)
{
	m_MySynth_01.noteOn(11, noteValue, velocity);
}

void Synthesizer_v1AudioProcessor::midiNoteOff(int noteValue)
{
	m_MySynth_01.noteOff(11, noteValue, 100, true);
}

void Synthesizer_v1AudioProcessor::clearVoices()
{
	m_MySynth_01.allNotesOff(11, false);
	m_MySynth_01.clearSounds();
	m_MySynth_01.clearVoices();
	Logger::getCurrentLogger()->writeToLog("Cleared voices");
}
