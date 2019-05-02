/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UnityDelayAudioProcessor::UnityDelayAudioProcessor()
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
	m_fDelayInSamples = 0;
	fDelayInMS = 0;
	m_fFeedback = 0;
	fFeedback_pct = 0;
	m_fWetLevel = 0;
	fWetLevel_pct = 0;
	m_SampleRate = 44100;
	
	m_ReadIndex = 0;
	m_WriteIndex = 0;
	m_dBuffer = NULL;
	m_BufferSize = 0;

	addParameter(delayTime = new AudioParameterFloat("delayTime", "DelayTime", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	delayTime->addListener(this);
	addParameter(feedBack_pct = new AudioParameterFloat("feedBack_pct", "FeedBack_Pct", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	feedBack_pct->addListener(this);
	addParameter(wetLevel_pct = new AudioParameterFloat("wetLevel_pct", "WetLevel_Pct", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
	wetLevel_pct->addListener(this);

	cookVariables();
}

UnityDelayAudioProcessor::~UnityDelayAudioProcessor()
{
	if (m_dBuffer)
		delete[] m_dBuffer;
}

void UnityDelayAudioProcessor::cookVariables()
{
	/*m_fFeedback = fFeedback_pct / 100;
	m_fWetLevel = fWetLevel_pct / 100;
	m_fDelayInSamples = fDelayInMS * ((float)m_SampleRate / 1000);

	//subtract to make read index
	m_ReadIndex = m_WriteIndex - (int)m_fDelayInSamples; //cast as an int for correct buffer position

	//check and wrap BACKWARDS if the index is negative
	if (m_ReadIndex < 0)
		m_ReadIndex += m_BufferSize; //amount of wrap is Read + Length*/
	m_lDelay.cookVariables(fDelayInMS, fFeedback_pct, fWetLevel_pct, m_SampleRate);
	m_rDelay.cookVariables(fDelayInMS, fFeedback_pct, fWetLevel_pct, m_SampleRate);
}

void UnityDelayAudioProcessor::resetDelay()
{
	/*//flush buffer
	if (m_dBuffer)
		memset(m_dBuffer, 0, m_BufferSize * sizeof(float));

	//init read/write indices
	m_WriteIndex = 0;
	m_ReadIndex = 0;*/
	m_lDelay.reset();
	m_rDelay.reset();
}

//==============================================================================
const String UnityDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UnityDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UnityDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UnityDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UnityDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UnityDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UnityDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UnityDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String UnityDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void UnityDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void UnityDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	/*m_SampleRate = sampleRate;
	
	m_BufferSize = 2 * m_SampleRate; //for a 2 second delay
	if (m_dBuffer)
		delete[] m_dBuffer;
	m_dBuffer = new float[m_BufferSize];*/
	m_lDelay.prepareDelay();
	m_rDelay.prepareDelay();

	//reset
	resetDelay();

	//then cook
	cookVariables();
}

void UnityDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UnityDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void UnityDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	auto* leftInBuffer = buffer.getReadPointer(0);
	auto* rightInBuffer = buffer.getReadPointer(1);

	auto* leftOutBuffer = buffer.getWritePointer(0);
	auto* rightOutBuffer = buffer.getWritePointer(1);

	
    for (int i = 0; i < buffer.getNumSamples(); i++)
	{
		//Mono-in. Stereo out (AUX effect)
		if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
			rightOutBuffer = leftOutBuffer; //copy to MONO
		else if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
		{
			leftOutBuffer[i] = m_lDelay.processDelay(leftInBuffer[i]);
			rightOutBuffer[i] = m_rDelay.processDelay(rightInBuffer[i]);
		}
	}
}

float UnityDelayAudioProcessor::dLinTerp(float x1, float x2, float y1, float y2, float x)
{
	float denom = x2 - x1;
	if (denom == 0)
		return y1; // should not ever happen

	// calculate decimal position of x
	float dx = (x - x1) / (x2 - x1);

	// use weighted sum method of interpolating
	float result = dx * y2 + (1 - dx)*y1;

	return result;
}

//==============================================================================
bool UnityDelayAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* UnityDelayAudioProcessor::createEditor()
{
    return new UnityDelayAudioProcessorEditor (*this);
}

//==============================================================================
void UnityDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UnityDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UnityDelayAudioProcessor();
}

void UnityDelayAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
	if (parameterIndex == 0)
	{
		//Logger::getCurrentLogger()->writeToLog("delayTime is changing");
		fDelayInMS = convertParamRange(newValue, 0.0f, 2000.0f);
	}
	else if (parameterIndex == 1)
	{
		fFeedback_pct = convertParamRange(newValue, 0.0f, 100.0f);
	}
	else
	{
		fWetLevel_pct = convertParamRange(newValue, 0.0f, 100.0f);
	}

	cookVariables();
}

void UnityDelayAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{

}

float UnityDelayAudioProcessor::convertParamRange(float oldValue, float newMin, float newMax)
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
