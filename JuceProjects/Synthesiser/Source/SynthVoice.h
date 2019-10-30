/*
  ==============================================================================

    SynthVoice.h
    Created: 15 Dec 2018 4:44:20pm
    Author:  AndyPandy

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"
#include "ADSR.h"
#include <math.h>

#define NUM_OF_OSCILLATORS 3

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound)
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}
	//==========================================================================
	void getOscType(float* selection)
	{
		theWave = *selection;
	}
	//==========================================================================
	void setOscStates(bool triangleEnabled, bool squareEnabled, bool subEnabled)
	{
		if (triangleEnabled)
			isOscEnabled[0] = true;
		else
			isOscEnabled[0] = false;

		if (squareEnabled)
			isOscEnabled[1] = true;
		else
			isOscEnabled[1] = false;

		if (subEnabled)
			isOscEnabled[2] = true;
		else
			isOscEnabled[2] = false;
	}
	//==========================================================================
	double getOscOutput()
	{
		double triangleOutput = isOscEnabled[0] ? triangleOsc.triangle(frequency) : 0;
		double squareOutput = isOscEnabled[1] ? squareOsc.square(frequency) : 0;
		double subOutput = isOscEnabled[2] ? subOsc.sinewave(subFrequency) : 0;

		double output = (triangleOutput + squareOutput + subOutput) / NUM_OF_OSCILLATORS;
		return output * masterLevel;
	}
	//==========================================================================
	void InitVoice()
	{
		envelope = new ADSR();

		envelope->setAttackRate(0.5 * sampleRate);
		envelope->setDecayRate(5 * sampleRate);
		envelope->setSustainLevel(.4);
		envelope->setReleaseRate(1 * sampleRate);

	}
	void setEnvelopeValues(float attack, float decay, float sustain, float release)
	{
		envelope->setAttackRate(attack * sampleRate);
		envelope->setDecayRate(decay * sampleRate);
		envelope->setSustainLevel(sustain);
		envelope->setReleaseRate(release * sampleRate);
	}
	double getEnvelopeOutput()
	{
		//double sound = env.adsr(getOscOutput(), env.trigger);
		double sound = getOscOutput() * envelope->process();
		return sound;
	}
	//==========================================================================
	void setMasterVolume(float value)
	{
		masterLevel = value;
	}
	//==========================================================================
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) 
	{
		Logger::getCurrentLogger()->writeToLog("startNote");
		envelope->gate(true);
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		subFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber - 12);
		//Logger::getCurrentLogger()->writeToLog("Midi note number: " + String(midiNoteNumber) + " Frequency: " + String(frequency));
	}
	//==========================================================================
	void stopNote(float velocity, bool allowTailOff)
	{
		envelope->gate(false);
		allowTailOff = true;

		if (velocity == 0)
			clearCurrentNote();
		Logger::getCurrentLogger()->writeToLog("Stop note");
	}
	//==========================================================================
	void pitchWheelMoved(int newPitchWheelValue)
	{

	}
	//==========================================================================
	void controllerMoved(int controllerNumber, int newControllerValue)
	{

	}
	//==========================================================================
	void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
	{	
		//Logger::getCurrentLogger()->writeToLog("Render next block");
		for (int sample = 0; sample < numSamples; ++sample)
		{
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
				outputBuffer.addSample(channel, startSample, getEnvelopeOutput());
			}
			++startSample;
		}
	}
	
private:
	float masterLevel;
	
	int theWave;
	double frequency = 400;
	double subFrequency = 400;

	maxiOsc triangleOsc;
	maxiOsc squareOsc;
	maxiOsc subOsc;
	bool isOscEnabled[3] = { false };

	ADSR *envelope;

	float sampleRate = 44100;

	
};