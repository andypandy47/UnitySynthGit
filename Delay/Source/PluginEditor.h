/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class UnityDelayAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    UnityDelayAudioProcessorEditor (UnityDelayAudioProcessor&);
    ~UnityDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void sliderValueChanged(Slider*) override;

private:
	Slider dtSlider;
	Slider fbSlider;
	Slider dwSlider;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UnityDelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnityDelayAudioProcessorEditor)
};
