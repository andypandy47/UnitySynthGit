/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UnitySchroederVerbAudioProcessorEditor::UnitySchroederVerbAudioProcessorEditor (UnitySchroederVerbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 400);

	preDelaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	preDelaySlider.setRange(0, 1);
	preDelaySlider.setValue(0.1);
	preDelaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	preDelaySlider.addListener(this);
	addAndMakeVisible(preDelaySlider);

	diffusionSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	diffusionSlider.setRange(0, 1);
	diffusionSlider.setValue(0.1);
	diffusionSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	diffusionSlider.addListener(this);
	addAndMakeVisible(diffusionSlider);

	dampingSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	dampingSlider.setRange(0, 1);
	dampingSlider.setValue(0.5);
	dampingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	dampingSlider.addListener(this);
	addAndMakeVisible(dampingSlider);

	rt60Slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	rt60Slider.setRange(0, 1);
	rt60Slider.setValue(0.2);
	rt60Slider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	rt60Slider.addListener(this);
	addAndMakeVisible(rt60Slider);

	wetSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	wetSlider.setRange(0, 1);
	wetSlider.setValue(0.3);
	wetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	wetSlider.addListener(this);
	addAndMakeVisible(wetSlider);

	drySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	drySlider.setRange(0, 1);
	drySlider.setValue(0.5);
	drySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	drySlider.addListener(this);
	addAndMakeVisible(drySlider);
}

UnitySchroederVerbAudioProcessorEditor::~UnitySchroederVerbAudioProcessorEditor()
{
}

//==============================================================================
void UnitySchroederVerbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void UnitySchroederVerbAudioProcessorEditor::resized()
{
	preDelaySlider.setBounds(100, 100, 100, 300);
	diffusionSlider.setBounds(200, 100, 100, 300);
	dampingSlider.setBounds(300, 100, 100, 300);
	rt60Slider.setBounds(400, 100, 100, 300);
	wetSlider.setBounds(500, 100, 100, 300);
	drySlider.setBounds(600, 100, 100, 300);
}

void UnitySchroederVerbAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &preDelaySlider)
	{
		processor.preDelay->setValueNotifyingHost(preDelaySlider.getValue());
	}
	else if (slider == &diffusionSlider)
	{
		processor.diffusion->setValueNotifyingHost(diffusionSlider.getValue());
	}
	else if (slider == &dampingSlider)
	{
		processor.damping->setValueNotifyingHost(dampingSlider.getValue());
	}
	else if (slider == &rt60Slider)
	{
		processor.rt60->setValueNotifyingHost(rt60Slider.getValue());
	}
	else if (slider == &wetSlider)
	{
		processor.wet->setValueNotifyingHost(wetSlider.getValue());
	}
	else
	{
		processor.dry->setValueNotifyingHost(drySlider.getValue());
	}
}
