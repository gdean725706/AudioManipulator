/*
  ==============================================================================

    RecordingBuffer.h
    Created: 11 Feb 2018 8:02:51pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "Phasor.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>


class RecordingBuffer
{
public:
	RecordingBuffer(float sampleRate = 44100, float bufferSize = 5) :
		m_sampleRate(sampleRate),
		m_bufferSize(sampleRate * bufferSize),
		m_audioBuffer(m_bufferSize),
		m_phasorSpeedMultiplier(1),
		m_phasorSpeed(sampleRate / m_audioBuffer.size()),
		m_playbackActive(false)
	{

	}

	void setSampleRate(float sampleRate)
	{
		m_sampleRate = sampleRate;
	}

	void fillBuffer(std::vector<float> data, int size)
	{
		m_audioBuffer.clear();
		m_audioBuffer.resize(size);
		m_audioBuffer = data;
		m_bufferSize = size;
		calculatePhasorSpeed();

	}

	float getSample(int index)
	{
		float r = m_audioBuffer[m_audioPhasor.getPhase() * m_bufferSize];
		m_audioPhasor.tick();
		return r;
	}

	void clearBuffer()
	{
		m_audioBuffer.clear();
	}

	void setSpeedMultiplier(float multiplier)
	{
		m_phasorSpeedMultiplier = multiplier;
		calculatePhasorSpeed();
	}

private:

	void calculatePhasorSpeed()
	{
		m_phasorSpeed = m_sampleRate / m_bufferSize;
		m_phasorSpeed *= m_phasorSpeedMultiplier;
	}

	float m_sampleRate;
	float m_bufferSize;
	Phasor m_audioPhasor;
	std::vector<float> m_audioBuffer;
	float m_phasorSpeedMultiplier;
	float m_phasorSpeed;
	bool m_playbackActive;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingBuffer)
};