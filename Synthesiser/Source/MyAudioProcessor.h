/*
  ==============================================================================

    MyAudioProcessor.h
    Created: 23 Apr 2019 1:54:38pm
    Author:  AndyPandy

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MyAudioProcessor : public juce::AudioProcessor
{
public:
	MyAudioProcessor() : juce::AudioProcessor() {}

	MyAudioProcessor(const BusesProperties& ioLayouts) :
		juce::AudioProcessor(ioLayouts)
	{}

	MyAudioProcessor(const std::initializer_list<const short[2]>& channelLayoutList)
		:
		juce::AudioProcessor(channelLayoutList)
	{}


	virtual void midiNoteOn(int noteValue, int velocity)
	{
	}

	virtual void midiNoteOff(int noteValue)
	{
	}

	virtual void clearVoices()
	{
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAudioProcessor)


};