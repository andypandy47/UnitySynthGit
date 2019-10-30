/*
  ==============================================================================

    Envelope.h
    Created: 18 Dec 2018 5:02:00pm
    Author:  AndyPandy

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Envelope    : public Component,
					public Slider::Listener
{
public:
    Envelope(Synthesizer_v1AudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;

	void sliderValueChanged(Slider*) override;

private:

	Synthesizer_v1AudioProcessor & processor;

	/*ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> attackTree;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> decayTree;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> sustainTree;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> releaseTree;*/

	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
