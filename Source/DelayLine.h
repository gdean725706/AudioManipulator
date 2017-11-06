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
	int m_maxDelay;
	int m_writeLocation;
	
public:
	DelayLine(int maxDelay);

	void clearBuffer();

	void writeSample(float sample);

	void tick();

	float getDelay(int delayTime);

	EffectType getType() override
	{
		return EffectType::Delay;
	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	double getTailLengthSeconds() const override;

};