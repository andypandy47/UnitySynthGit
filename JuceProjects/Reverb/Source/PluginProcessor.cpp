/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UnitySchroederVerbAudioProcessor::UnitySchroederVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	addParameter(preDelay = new AudioParameterFloat("preDelay", "PreDelay", NormalisableRange<float>(0.0f, 1.0f), 0.2f));
	preDelay->addListener(this);
	
	addParameter(diffusion = new AudioParameterFloat("diffusion", "Diffusion", NormalisableRange<float>(0.0f, 1.0f), 0.2f));
	diffusion->addListener(this);

	addParameter(damping = new AudioParameterFloat("damping", "Damping", NormalisableRange<float>(0.0f, 1.0f), 0.2f));
	damping->addListener(this);

	addParameter(rt60 = new AudioParameterFloat("rt60", "Rt60", NormalisableRange<float>(0.0f, 1.0f), 0.2f));
	rt60->addListener(this);

	addParameter(wet = new AudioParameterFloat("wet", "Wet", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	wet->addListener(this);

	addParameter(dry = new AudioParameterFloat("dry", "dry", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	dry->addListener(this);

	schro.setBypass(false);
	filters.setup(sampleRate, 500);
	m_PreDelay = 5.0f;
	m_Wet = 0.3f;
	m_Rt60 = 2.0f;
	m_Diffusion = 0.0f;
	m_Dry = 0.5f;
	m_Damping = 5000.0f;
	schro.setDecayFactor(m_Rt60);
}

UnitySchroederVerbAudioProcessor::~UnitySchroederVerbAudioProcessor()
{
}

//==============================================================================
const String UnitySchroederVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UnitySchroederVerbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UnitySchroederVerbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UnitySchroederVerbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UnitySchroederVerbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UnitySchroederVerbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UnitySchroederVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UnitySchroederVerbAudioProcessor::setCurrentProgram (int index)
{
}

const String UnitySchroederVerbAudioProcessor::getProgramName (int index)
{
    return {};
}

void UnitySchroederVerbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void UnitySchroederVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void UnitySchroederVerbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UnitySchroederVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void UnitySchroederVerbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	auto* channelDataLeft = buffer.getWritePointer(0);
	auto* channelDataRight = buffer.getWritePointer(1);

	float outL = 0.0f, outR = 0.0f;

	//stereo interleaved so process left and right
	//then step through in increments of 2??
	for (int i = 0; i < numSamples; i++)
	{
		float in = filters.filter(channelDataLeft[i]), in_1 = filters.filter(channelDataRight[i]);

		//seperate left and right processing
		outL = m_Wet * schro.next(in) + m_Dry * in;
		outR = m_Wet * schro.next(in_1) + m_Dry * in_1;

		channelDataLeft[i] = outL;

		//Mono-in, Stereo-out
	//	if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
	//		channelDataRight[i + 1] = channelDataLeft[i]; //copy to mono

		if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
			channelDataRight[i] = outR;

	}
}

//==============================================================================
bool UnitySchroederVerbAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* UnitySchroederVerbAudioProcessor::createEditor()
{
    return new UnitySchroederVerbAudioProcessorEditor (*this);
}

//==============================================================================
void UnitySchroederVerbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UnitySchroederVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UnitySchroederVerbAudioProcessor();
}

void UnitySchroederVerbAudioProcessor::parameterValueChanged(int paramterIndex, float newValue)
{
	if (paramterIndex == 0)
	{
		m_PreDelay = convertParamRange(newValue, 0.0f, 100.0f); //this doesnt effect anything either??
	}
	else if (paramterIndex == 1)
	{
		m_Diffusion = convertParamRange(newValue, 0.0f, 100.0f); //this doesnt even do anything
	}
	else if (paramterIndex == 2)
	{
		m_Damping = convertParamRange(newValue, 200.0f, 20000.0f);//in hz??
		filters.setup(sampleRate, m_Damping);
	}
	else if (paramterIndex == 3)
	{
		m_Rt60 = convertParamRange(newValue, 0.0f, 5.0f); //in seconds
		schro.setDecayFactor(m_Rt60);
	}
	//wet and dry are already in correct range and dont need their range converting
	else if (paramterIndex == 4)
	{
		m_Wet = newValue;
	}
	else
	{
		m_Dry = newValue;
	}
}

void UnitySchroederVerbAudioProcessor::parameterGestureChanged(int paramterIndex, bool gestureIsStarting)
{

}

float UnitySchroederVerbAudioProcessor::convertParamRange(float oldValue, float newMin, float newMax)
{
	//This is true for all AudioParameterFloats
	float oldMin = 0.0f;
	float oldMax = 1.0f;

	float oldRange = oldMax - oldMin;
	float newRange = newMax - newMin;
	float newValue = (((oldValue - oldMin) * newRange) / oldRange) + newMin;

	Logger::getCurrentLogger()->writeToLog(String(newValue));
	return newValue;
}