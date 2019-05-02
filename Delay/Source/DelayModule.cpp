/*
  ==============================================================================

    DelayModule.cpp
    Created: 26 Mar 2019 6:57:58pm
    Author:  AndyPandy

  ==============================================================================
*/

#include "DelayModule.h"

DelayModule::DelayModule()
{
	m_fDelayInSamples = 0;
	m_fFeedback = 0;
	m_fWetLevel = 0;
	m_SampleRate = 44100;

	m_ReadIndex = 0;
	m_WriteIndex = 0;
	m_dBuffer = NULL;
	m_BufferSize = 0;
}

DelayModule::~DelayModule()
{
	if (m_dBuffer)
		delete[] m_dBuffer;
}

void DelayModule::cookVariables(float delayInMS, float fbInPercent, float dryWeyInPercent, double sampleRate)
{
	m_SampleRate = sampleRate;

	m_fFeedback = fbInPercent / 100;
	m_fWetLevel = dryWeyInPercent / 100;
	m_fDelayInSamples = delayInMS * ((float)m_SampleRate / 1000);

	//subtract to make read index
	m_ReadIndex = m_WriteIndex - (int)m_fDelayInSamples; //cast as an int for correct buffer position

	//check and wrap BACKWARDS if the index is negative
	if (m_ReadIndex < 0)
		m_ReadIndex += m_BufferSize; //amount of wrap is Read + Length
}

void DelayModule::reset()
{
	//flush buffer
	if (m_dBuffer)
		memset(m_dBuffer, 0, m_BufferSize * sizeof(float));

	//init read/write indices
	m_WriteIndex = 0;
	m_ReadIndex = 0;
}

void DelayModule::prepareDelay()
{
	m_BufferSize = 2 * m_SampleRate; //for a 2 second delay
	if (m_dBuffer)
		delete[] m_dBuffer;
	m_dBuffer = new float[m_BufferSize];
}

float DelayModule::dLinTerp(float x1, float x2, float y1, float y2, float x)
{
	float denom = x2 - x1;
	if (denom == 0)
		return y1; // should not ever happen

	// calculate decimal position of x
	float dx = (x - x1) / (x2 - x1);

	// use weighted sum method of interpolating
	float result = dx * y2 + (1 - dx)*y1;

	return result;
}

float DelayModule::processDelay(float inSample)
{
	float xn = inSample;

	//Read the output of the delay at m_ReadIndex
	float yn = m_dBuffer[m_ReadIndex];

	//if delay is < 1 sample, interpolate between input x(n) and x(n-1)
	if (m_ReadIndex == m_WriteIndex && m_fDelayInSamples < 1.00)
	{
		//interpolate x(n) with x(n-1), set yn = xn
		yn = xn;
	}

	//Read the location one behind yn at y(n-1)
	int nReadIndex_1 = m_ReadIndex - 1;
	if (nReadIndex_1 < 0)
		nReadIndex_1 = m_BufferSize - 1; //buffer size - 1 is last location

	//get y(n-1)
	float yn_1 = m_dBuffer[nReadIndex_1];

	//interpolate: (0, yn) and (1, yn-1) by the amount fracDelay
	float fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;

	//linerp: x1, x2, y1, y2, x
	float fInterp = dLinTerp(0, 1, yn, yn_1, fFracDelay); //interp frac between them

	//if zero delay, just pass input to output
	if (m_fDelayInSamples == 0)
		yn = xn;
	else
		yn = fInterp;

	//write input to the delay
	m_dBuffer[m_WriteIndex] = xn + m_fFeedback * yn;

	//increment pointers and wrap if necessary
	m_WriteIndex++;
	if (m_WriteIndex >= m_BufferSize)
		m_WriteIndex = 0;
	m_ReadIndex++;
	if (m_ReadIndex >= m_BufferSize)
		m_ReadIndex = 0;

	return m_fWetLevel * yn + (1.0 - m_fWetLevel) * xn;
}
