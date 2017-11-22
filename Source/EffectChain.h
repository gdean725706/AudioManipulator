/*
  ==============================================================================

    EffectChain.h
    Created: 6 Nov 2017 7:18:17pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayLine.h"

class EffectChain
{
public:
	EffectChain() :
		m_leftDelay(0, 44100*5),
		m_rightDelay(0, 44100*5)
	{
		m_xVal = 0;
		m_yVal = 0;
	}

	void processDelay(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		m_leftDelay.processBlock(buffer, midiMessages);
		m_rightDelay.processBlock(buffer, midiMessages);
	}

	void setXY(float x, float y)
	{
		m_xVal = x;
		m_yVal = y;
	}

private:

	DelayLine m_leftDelay, m_rightDelay;
	float m_xVal, m_yVal;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectChain)
};