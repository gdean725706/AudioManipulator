/*
  ==============================================================================

    DelayLine.h
    Created: 6 Nov 2017 6:08:07pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectBase.h"

class DelayLine : public EffectBase
{
private:
	ScopedPointer<float> m_delayBuffer;
	int m_maxDelay, m_writeLocation, m_channel;
	float m_feedbackLevel, m_delayTime;
	double m_sampleRate;

public:
	DelayLine(int channel, int maxDelay);

	void clearBuffer();

	void writeSample(float sample);

	void tick();

	float getDelay(int delayTime);

	EffectType getType() override
	{
		return EffectType::Delay;
	}

	void setFeedbackLevel(float feedbackLevel)
	{
		m_feedbackLevel = feedbackLevel;
	}

	void setDelayTime(float delayTime)
	{
		m_delayTime = delayTime;
	}

	void setMaxDelayLength(float maxDelay)
	{
		m_maxDelay = maxDelay;
	}

	void setChannel(int channel)
	{
		m_channel = channel;
	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	double getTailLengthSeconds() const override;

};