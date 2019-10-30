/*
  ==============================================================================

    DelayModule.h
    Created: 26 Mar 2019 6:57:58pm
    Author:  AndyPandy

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//==============================================================================
/**
*/
class DelayModule
{
public:
	DelayModule();
	~DelayModule();
//==============================================================================
	float processDelay(float inSample);
	void cookVariables(float delayInMS, float fbInPercent, float dryWeyInPercent, double sampleRate);
	void reset();
	void prepareDelay();
	float dLinTerp(float x1, float x2, float y1, float y2, float x);
public:

private:
	double m_SampleRate;

	float m_fDelayInSamples;
	float m_fFeedback;
	float m_fWetLevel;

	float* m_dBuffer;
	int m_ReadIndex;
	int m_WriteIndex;
	int m_BufferSize;
};
