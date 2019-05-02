/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope.h"

//==============================================================================
/**
*/
class Synthesizer_v1AudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    Synthesizer_v1AudioProcessorEditor (Synthesizer_v1AudioProcessor&);
    ~Synthesizer_v1AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void sliderValueChanged(Slider*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Synthesizer_v1AudioProcessor& processor;
	
	Envelope envGUI;

	Slider masterVolumeSlider;
	Slider cutoffSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesizer_v1AudioProcessorEditor)
};
