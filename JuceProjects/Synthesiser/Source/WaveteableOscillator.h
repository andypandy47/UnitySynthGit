/*
  ==============================================================================

    WaveteableOscillator.h
    Created: 2 Feb 2019 2:26:36pm
    Author:  AndyPandy

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class WaveTableOscillator
{
public:
	enum OscType
	{
		Sine,
		Square,
		Triangle
	};

	WaveTableOscillator(OscType type)
	{
		//jassert(wavetable.getNumChannels() == 1);
		
		switch (type)
		{
		case OscType::Sine:
			createSineTable();
			break;
		case OscType::Square:
			createSquareTable();
			break;
		case OscType::Triangle:
			createTriangleTable();
			break;
		}
		
	}

	void setFrequency(float frequency, float sampleRate)
	{
		auto tableSizeOverSampleRate = m_TableSize / sampleRate;
		m_TableData = frequency * tableSizeOverSampleRate;
	}

	forcedinline float getNextSample() noexcept
	{
		auto index0 = (unsigned int)m_CurrentIndex;
		auto index1 = index0 + 1;

		auto frac = m_CurrentIndex - (float)index0;

		auto* table = m_Wavetable.getReadPointer(0);
		auto value0 = table[index0];
		auto value1 = table[index1];

		auto currentSample = value0 + frac * (value1 - value0);

		if ((m_CurrentIndex += m_TableData) > m_TableSize)
			m_CurrentIndex -= m_TableSize;

		return currentSample;
	}

private:
	void createSineTable()
	{
		m_Wavetable.setSize(1, m_TableSize + 1);
		m_Wavetable.clear();

		auto* samples = m_Wavetable.getWritePointer(0);
		
		int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };
		float harmonicWeights[] = { 0.5f, 0.1f, 0.05f, 0.125f, 0.09f, 0.005, 0.002f, 0.001f };

		jassert(numElementsInArray(harmonics) == numElementsInArray(harmonicWeights));

		for (auto harmonic = 0; harmonic < numElementsInArray(harmonics); ++harmonic)
		{
			auto angleDelta = MathConstants<double>::twoPi / (double)(m_TableSize - 1) * harmonics[harmonic];
			auto currentAngle = 0.0;

			for (auto i = 0; i < m_TableSize; ++i)
			{
				auto sample = std::sin(currentAngle);
				samples[i] += (float)sample * harmonicWeights[harmonic];
				currentAngle += angleDelta;
			}
		}

		samples[m_TableSize] = samples[0];
	}

	void createSquareTable()
	{
		m_Wavetable.setSize(1, m_TableSize + 1);
		m_Wavetable.clear();

		auto* samples = m_Wavetable.getWritePointer(0);

		int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };
		float harmonicWeights[] = { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

		jassert(numElementsInArray(harmonics) == numElementsInArray(harmonicWeights));

		for (auto harmonic = 0; harmonic < numElementsInArray(harmonics); ++harmonic)
		{
			auto angleDelta = MathConstants<double>::twoPi / (double)(m_TableSize - 1) * harmonics[harmonic];
			auto currentAngle = 0.0;

			for (auto i = 0; i < m_TableSize; ++i)
			{
				auto sample = std::sin(currentAngle);
				samples[i] += (float)sample * harmonicWeights[harmonic];
				currentAngle += angleDelta;
			}
		}

		samples[m_TableSize] = samples[0];
	}

	void createTriangleTable()
	{
		m_Wavetable.setSize(1, m_TableSize + 1);
		m_Wavetable.clear();

		auto* samples = m_Wavetable.getWritePointer(0);

		int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };
		float harmonicWeights[] = { 0.25f, 0.5f, 0.75f, 1.0f, 0.75f, 0.5f, 0.25f, 0.0f };

		jassert(numElementsInArray(harmonics) == numElementsInArray(harmonicWeights));

		for (auto harmonic = 0; harmonic < numElementsInArray(harmonics); ++harmonic)
		{
			auto angleDelta = MathConstants<double>::twoPi / (double)(m_TableSize - 1) * harmonics[harmonic];
			auto currentAngle = 0.0;

			for (auto i = 0; i < m_TableSize; ++i)
			{
				auto sample = std::sin(currentAngle);
				samples[i] += (float)sample * harmonicWeights[harmonic];
				currentAngle += angleDelta;
			}
		}

		samples[m_TableSize] = samples[0];
	}

private:
	AudioSampleBuffer m_Wavetable;
	int m_TableSize = 1 << 7;
	float m_CurrentIndex = 0.0f, m_TableData = 0.0f;

};
