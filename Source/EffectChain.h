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
#include "PitchShifter.h"
#include "RingModulator.h"

class EffectChain
{
public:
	EffectChain(AudioProcessor* mainProcessor) :
		m_delayLine(44100*5, mainProcessor),
		m_flanger(44100*40, mainProcessor),
		m_pitchShifter(mainProcessor),
		m_ringMod(mainProcessor),
		m_effects(0)
	{
		m_xVal = 0;
		m_yVal = 0;

		m_effects.push_back(&m_delayLine);
		m_effects.push_back(&m_flanger);
		m_effects.push_back(&m_pitchShifter);
		m_effects.push_back(&m_ringMod);

	}

	void prepareToPlay(double sampleRate, int samplesPerBlock)
	{
		m_delayLine.prepareToPlay(sampleRate, samplesPerBlock);
		m_flanger.prepareToPlay(sampleRate, samplesPerBlock);
		m_pitchShifter.prepareToPlay(sampleRate, samplesPerBlock);
	}

	void releaseResources()
	{
		m_delayLine.releaseResources();
		m_flanger.releaseResources();
		m_pitchShifter.releaseResources();
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

	void processPitchShifter(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		m_pitchShifter.processBlock(buffer, midiMessages);
	}

	void processRingMod(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		m_ringMod.processBlock(buffer, midiMessages);
	}

	void process(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		for (int i = 0; i < m_effects.size(); ++i)
		{
			if (m_effects[i]->isActive())
				m_effects[i]->processBlock(buffer, midiMessages);
		}
	}

	void setEffectState(EffectBase::EffectType effect, bool state)
	{
		for (int i = 0; i < m_effects.size(); ++i)
		{
			if (m_effects[i]->getType() == effect)
			{
				m_effects[i]->setActive(state);
			}
		}
	}

	// Called from an X-Y controller with a reference
	void setXY(float x, float y)
	{
		m_delayLine.updateAudioParameters(x, y);

		m_flanger.updateAudioParameters(x, y);

		m_pitchShifter.setFrequency(scaleRange(y, 0.0f, 1.0f, -12.0f, 12.0f));

		m_ringMod.updateAudioParameters(x, y);


		m_xVal = x;
		m_yVal = y;
	}
	
	void updateMapping(EffectBase* effect, int xMap, int yMap)
	{
		effect->setParameterMapping(xMap, yMap);
	}

	std::vector<EffectBase*> getAllEffects()
	{
		return m_effects;
	}

private:
	typedef EffectBase::EffectType FXType;

	StereoDelay m_delayLine;
	Flanger m_flanger;
	PitchShifter m_pitchShifter;
	RingModulator m_ringMod;

	float m_xVal, m_yVal;
	AudioProcessor* m_processor;
	std::vector<EffectBase*> m_effects;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectChain)
};