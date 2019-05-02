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
class UnitySchroederVerbAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    UnitySchroederVerbAudioProcessorEditor (UnitySchroederVerbAudioProcessor&);
    ~UnitySchroederVerbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider*) override;

private:
	Slider preDelaySlider;
	Slider diffusionSlider;
	Slider dampingSlider;
	Slider rt60Slider;
	Slider wetSlider;
	Slider drySlider;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UnitySchroederVerbAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySchroederVerbAudioProcessorEditor)
};
