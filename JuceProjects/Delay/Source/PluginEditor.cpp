/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UnityDelayAudioProcessorEditor::UnityDelayAudioProcessorEditor (UnityDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);

	dtSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	dtSlider.setRange(0, 1);
	dtSlider.setValue(0.5);
	dtSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	dtSlider.setTextValueSuffix("mSec");
	dtSlider.addListener(this);
	addAndMakeVisible(dtSlider);

	fbSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	fbSlider.setRange(0, 1);
	fbSlider.setValue(0.1);
	fbSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	fbSlider.setTextValueSuffix("%");
	fbSlider.addListener(this);
	addAndMakeVisible(fbSlider);

	dwSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	dwSlider.setRange(0, 1);
	dwSlider.setValue(0.5);
	dwSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 30);
	dwSlider.setTextValueSuffix("%");
	dwSlider.addListener(this);
	addAndMakeVisible(dwSlider);
}

UnityDelayAudioProcessorEditor::~UnityDelayAudioProcessorEditor()
{
}

//==============================================================================
void UnityDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	
}

void UnityDelayAudioProcessorEditor::resized()
{
	dtSlider.setBounds(100, 100, 100, 300);
	fbSlider.setBounds(200, 100, 100, 300);
	dwSlider.setBounds(300, 100, 100, 300);
}

void UnityDelayAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &dtSlider)
	{
		processor.delayTime->setValueNotifyingHost(dtSlider.getValue());
	}
	else if (slider == &fbSlider)
	{
		processor.feedBack_pct->setValueNotifyingHost(fbSlider.getValue());
	}
	else
	{
		processor.wetLevel_pct->setValueNotifyingHost(dwSlider.getValue());
	}
}
