/*
  ==============================================================================

    Envelope.cpp
    Created: 18 Dec 2018 5:02:00pm
    Author:  AndyPandy

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(Synthesizer_v1AudioProcessor& p) :
processor(p)
{
	setSize(200, 200);

	attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	attackSlider.setRange(0.0f, 1.0f);
	attackSlider.setValue(0.5f);
	attackSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	attackSlider.addListener(this);
	addAndMakeVisible(attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	decaySlider.setRange(0.0f, 1.0f);
	decaySlider.setValue(0.5f);
	decaySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	decaySlider.addListener(this);
	addAndMakeVisible(decaySlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	sustainSlider.setRange(0.0f, 1.0f);
	sustainSlider.setValue(0.5f);
	sustainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	sustainSlider.addListener(this);
	addAndMakeVisible(sustainSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	releaseSlider.setRange(0.0f, 1.0f);
	releaseSlider.setValue(0.5f);
	releaseSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	releaseSlider.addListener(this);
	addAndMakeVisible(releaseSlider);

	/*attackTree = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "attack", attackSlider);
	decayTree = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "decay", decaySlider);
	sustainTree = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "sustain", sustainSlider);
	releaseTree = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "release", releaseSlider);*/

}

Envelope::~Envelope()
{
}

void Envelope::paint (Graphics& g)
{
	g.fillAll(Colours::black);
}

void Envelope::resized()
{
	Rectangle<int> area = getLocalBounds().reduced(40);

	attackSlider.setBounds(10, 10, 60, 200);
	decaySlider.setBounds(50, 10, 60, 200);
	sustainSlider.setBounds(90, 10, 60, 200);
	releaseSlider.setBounds(130, 10, 60, 200);

}

void Envelope::sliderValueChanged(Slider* slider)
{
	if (slider == &attackSlider)
	{
		processor.attackAmount->setValueNotifyingHost(attackSlider.getValue());
	}
	else if (slider == &decaySlider)
	{
		processor.decayAmount->setValueNotifyingHost(decaySlider.getValue());
	}
	else if (slider == &sustainSlider)
	{
		processor.sustainAmount->setValueNotifyingHost(sustainSlider.getValue());
	}
	else if (slider == &releaseSlider)
	{
		processor.releaseAmount->setValueNotifyingHost(releaseSlider.getValue());
	}
}
