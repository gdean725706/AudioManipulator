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
#include "Flanger.h"
#include "MainProcessor.h"

class EffectChain
{
public:
	EffectChain(AudioProcessor* mainProcessor) :
		m_delayLine(44100*5, mainProcessor),
		m_flanger(44100*40, mainProcessor)
	{
		m_xVal = 0;
		m_yVal = 0;
	}

	void prepareToPlay(double sampleRate, int samplesPerBlock)
	{
		m_delayLine.prepareToPlay(sampleRate, samplesPerBlock);
		m_flanger.prepareToPlay(sampleRate, samplesPerBlock);
	}

	void releaseResources()
	{
		m_delayLine.releaseResources();
		m_flanger.releaseResources();
	}

	// Call this each block to process the delays
	void processDelay(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		m_delayLine.processBlock(buffer, midiMessages);
	}

	void processFlanger(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		m_flanger.processBlock(buffer, midiMessages);
	}

	void setXY(float x, float y)
	{
		m_delayLine.updateAudioParameters(x, y);

		m_xVal = x;
		m_yVal = y;
	}

	void updateParameterMappings(EffectBase::EffectType effect, int xMap, int yMap)
	{
		switch (effect)
		{
			case FXType::Delay:
			{
				m_delayLine.setParameterMapping(xMap, yMap);
				break;
			}
			case FXType::Flanger:
			{
				m_flanger.setParameterMapping(xMap, yMap);
				break;
			}
		}
	}

private:
	typedef EffectBase::EffectType FXType;
	StereoDelay m_delayLine;
	Flanger m_flanger;
	float m_xVal, m_yVal;
	AudioProcessor* m_processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectChain)
};