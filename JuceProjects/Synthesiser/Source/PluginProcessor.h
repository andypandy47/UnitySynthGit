/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#ifdef PLUGINPROCESSOR_EXPORT
#define PLUGINPROCESSOR_API __declspec(dllexport)
#else
#define	PLUGINPROCESSOR_API __declspec(dllimport)
#endif

#pragma once

#define Voice_Count 15

//#include "../JuceLibraryCode/JuceHeader.h"
#include "MyAudioProcessor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
/**
*/
class Synthesizer_v1AudioProcessor  : public MyAudioProcessor, private AudioProcessorParameter::Listener
{
public:
    //==============================================================================
    Synthesizer_v1AudioProcessor();
    ~Synthesizer_v1AudioProcessor();

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
	void parameterValueChanged(int paramterIndex, float newValue) override;
	void parameterGestureChanged(int parameterInex, bool gestureIsStarting) override;

	//==============================================================================
	//int ParseMidiNote(float paramValue);

	void midiNoteOn(int noteValue, int velocity) override;
	void midiNoteOff(int noteValue) override;
	void clearVoices() override;

	//==============================================================================
	void updateFilter();

	//==============================================================================
	
	AudioParameterFloat* triangleOnOff;
	AudioParameterFloat* squareOnOff;
	AudioParameterFloat* subOnOff;

	AudioParameterFloat* filterTypeValue;
	AudioParameterFloat* filterCuttoffAmount;
	AudioParameterFloat* filterResAmount;

	AudioParameterFloat* attackAmount;
	AudioParameterFloat* decayAmount;
	AudioParameterFloat* sustainAmount;
	AudioParameterFloat* releaseAmount;
	
	AudioParameterFloat* masterVolume;

private:
	SynthVoice* m_MyVoice_01;
	Synthesiser m_MySynth_01;

	Array<AudioParameterFloat*> oscStateVals_01;
	bool oscStates[3] = { false };

	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
	int filterType = 0;
	float cutoffVal = 100.0f;
	float resVal = 1.0f;

	float attackTime = 100.0f;
	float decayTime = 100.0f;
	float sustainLevel = 100.0f;
	float releaseTime = 100.0f;

	double lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesizer_v1AudioProcessor)
};
