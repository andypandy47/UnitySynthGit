/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Iir.h"
#include "Schroeder.h"

//==============================================================================
/**
*/
class UnitySchroederVerbAudioProcessor  : public AudioProcessor, public AudioProcessorParameter::Listener
{
public:
    //==============================================================================
    UnitySchroederVerbAudioProcessor();
    ~UnitySchroederVerbAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void parameterValueChanged(int paramterIndex, float newValue) override;
	void parameterGestureChanged(int parameterInex, bool gestureIsStarting) override;

	float convertParamRange(float oldValue, float newMin, float newMax);

	Schroeder schro;
	int sampleRate = 44100;

	Iir::Butterworth::LowPass<4> filters;

	AudioParameterFloat* preDelay;
	AudioParameterFloat* diffusion;
	AudioParameterFloat* damping;
	AudioParameterFloat* rt60;
	AudioParameterFloat* wet;
	AudioParameterFloat* dry;

private:
	
	float m_PreDelay, m_Wet, m_Rt60, m_Diffusion, m_Dry, m_Damping;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySchroederVerbAudioProcessor)
};
