/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synthesizer_v1AudioProcessorEditor::Synthesizer_v1AudioProcessorEditor (Synthesizer_v1AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGUI(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

	addAndMakeVisible(&envGUI);	

	cutoffSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	cutoffSlider.setRange(0.0f, 1.0f);
	cutoffSlider.setValue(1.0f);
	cutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	cutoffSlider.addListener(this);
	addAndMakeVisible(cutoffSlider);

	masterVolumeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	masterVolumeSlider.setRange(0.0f, 1.0f);
	masterVolumeSlider.setValue(0.5f);
	masterVolumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	masterVolumeSlider.addListener(this);
	addAndMakeVisible(masterVolumeSlider);
}

Synthesizer_v1AudioProcessorEditor::~Synthesizer_v1AudioProcessorEditor()
{
}

//==============================================================================
void Synthesizer_v1AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::white);

}

void Synthesizer_v1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	Rectangle<int> area = getLocalBounds();
	
	const int componentWidth = 200;
	const int componentHeight = 225;

	envGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

	cutoffSlider.setBounds(600, 200, 50, 300);
	masterVolumeSlider.setBounds(650, 200, 50, 300);
}

void Synthesizer_v1AudioProcessorEditor::sliderValueChanged(Slider* slider)
{

	if (slider == &cutoffSlider)
	{
		processor.filterCuttoffAmount->setValueNotifyingHost(cutoffSlider.getValue());
		//Logger::getCurrentLogger()->writeToLog(String(processor.filterCuttoffAmount->get()));
	}

	if (slider == &masterVolumeSlider)
	{
		processor.masterVolume->setValueNotifyingHost(masterVolumeSlider.getValue());
	}
}
