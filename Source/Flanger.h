/*
  ==============================================================================

    Flanger.h
    Created: 7 Dec 2017 12:21:37am
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectBase.h"
#include "DelayUnit.h"

//==============================================================================
/*
*/
class Flanger : public EffectBase
{
private:
	double m_sampleRate;
	AudioParameterFloat* m_feedback, *m_depth, *m_frequency;
	DelayUnit m_leftDelay, m_rightDelay;
	float m_phase;

public:
    Flanger(int maxDelay);
    ~Flanger();

	EffectType getType() override
	{
		return EffectType::Flanger;
	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	double getTailLengthSeconds() const override;
};
