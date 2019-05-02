/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayModule.h"

//==============================================================================
/**
*/
class UnityDelayAudioProcessor  : public AudioProcessor, private AudioProcessorParameter::Listener
{
public:
    //==============================================================================
    UnityDelayAudioProcessor();
    ~UnityDelayAudioProcessor();

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
	//==============================================================================
	void cookVariables();
	void resetDelay();
	//==============================================================================
	float dLinTerp(float x1, float x2, float y1, float y2, float x);

	void parameterValueChanged(int paramterIndex, float newValue) override;
	void parameterGestureChanged(int parameterInex, bool gestureIsStarting) override;
	float convertParamRange(float paramValue, float newMin, float newMax);

	float fDelayInMS;
	float fFeedback_pct;
	float fWetLevel_pct;

	AudioParameterFloat* delayTime;
	AudioParameterFloat* feedBack_pct;
	AudioParameterFloat* wetLevel_pct;

private:
	double m_SampleRate;

	float m_fDelayInSamples;
	float m_fFeedback;
	float m_fWetLevel;

	float* m_dBuffer;
	int m_ReadIndex;
	int m_WriteIndex;
	int m_BufferSize;

	DelayModule m_lDelay;
	DelayModule m_rDelay;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnityDelayAudioProcessor)
};
