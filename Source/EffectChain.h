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
#include "Chorus.h"
#include "Flanger.h"
#include "MainProcessor.h"
#include "Harmonizer.h"
#include "RingModulator.h"

class EffectChain
{
public:
	EffectChain(AudioProcessor* mainProcessor) :
		m_delayLine(44100 * 5, mainProcessor),
		m_chorus(mainProcessor),
		m_flanger(44100 * 40, mainProcessor),
		m_pitchShifter(mainProcessor),
		m_ringMod(mainProcessor),
		m_harmonizer(mainProcessor),
		m_effects(0),
		m_lastXName("XName"),
		m_lastYName("YName"),
		m_updateName(true)
	{
		m_xVal = 0;
		m_yVal = 0;

		m_effects.push_back(&m_delayLine);
		m_effects.push_back(&m_chorus);
		m_effects.push_back(&m_flanger);
		m_effects.push_back(&m_pitchShifter);
		m_effects.push_back(&m_ringMod);
		m_effects.push_back(&m_harmonizer);

	}

	void prepareToPlay(double sampleRate, int samplesPerBlock)
	{
		for (auto* effect : m_effects)
		{
			effect->prepareToPlay(sampleRate, samplesPerBlock);
		}

	}

	void releaseResources()
	{
		for (auto* effect : m_effects)
		{
			effect->releaseResources();
		}
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
		m_updateName = true;
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
		for (auto* effect : m_effects)
		{
			effect->updateAudioParameters(x,y);
		}

		m_pitchShifter.setFrequency(scaleRange(y, 0.0f, 1.0f, -12.0f, 12.0f));
		m_harmonizer.updateFrequencies();

		m_xVal = x;
		m_yVal = y;
	}

	// Updates X - Y control labels
	void getLatestNames(String &x, String &y)
	{
		if (m_updateName)
		{
			x.clear();
			y.clear();
			for (auto* effect : m_effects)
			{
				if (effect->isActive())
				{
					String x1, y1;
					effect->getCurrentParameterNames(x1, y1);
					x.append(x1 + "\n", x1.length() + 3);
					y.append(y1 + "\n", y1.length() + 3);
				}
			}
			m_updateName = false;
		}
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
	Chorus m_chorus;
	Flanger m_flanger;
	PitchShifter m_pitchShifter;
	RingModulator m_ringMod;
	Harmonizer m_harmonizer;

	float m_xVal, m_yVal;
	AudioProcessor* m_processor;
	std::vector<EffectBase*> m_effects;

	String m_lastXName, m_lastYName;
	bool m_updateName;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectChain)
};