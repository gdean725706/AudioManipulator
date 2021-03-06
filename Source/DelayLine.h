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
#include "DelayUnit.h"

class StereoDelay : public EffectBase
{
private:
	double m_sampleRate;

	// Audio params
	AudioParameterFloat* m_feedbackLevel, *m_delayTime;
	DelayUnit m_leftDelay, m_rightDelay;

public:
	StereoDelay(int maxDelay, AudioProcessor* processor);
	~StereoDelay();

	EffectType getType() override
	{
		return EffectType::Delay;
	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	double getTailLengthSeconds() const override;

};