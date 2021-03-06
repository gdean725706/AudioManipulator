/*
  ==============================================================================

    EffectBase.h
    Created: 21 Oct 2017 4:02:47pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <map>

template <class T>
T scaleRange(T input, T inputStart, T inputEnd, T outputStart, T outputEnd)
{
	return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
}


class EffectBase : public AudioProcessor
{
public:
	EffectBase(AudioProcessor* processor) :
		m_xParameter(0),
		m_yParameter(1),
		m_xVal(0),
		m_yVal(0),
		m_processor(processor)
	{

	}

	enum EffectType
	{
		LowPassFilter,
		HighPassFilter,
		Phaser,
		Delay,
		Chorus,
		Flanger,
		Speed,
		PitchShift,
		RingMod,
		Harmonizer
	};

	void setActive(bool active)
	{
		m_active = active;
	}

	bool isActive()
	{
		return m_active;
	}

	// Apply audio parameters X and Y based on their mappings to the parameter vector
	void updateAudioParameters(float x, float y)
	{
		if (!m_parameterVector.empty())
		{
			x = scaleRange(x, 0.0f, 1.0f, m_parameterVector[m_xParameter]->range.start, m_parameterVector[m_xParameter]->range.end);
			y = scaleRange(y, 0.0f, 1.0f, m_parameterVector[m_yParameter]->range.start, m_parameterVector[m_yParameter]->range.end);

			*m_parameterVector[m_xParameter] = (x);
			*m_parameterVector[m_yParameter] = (y);
		}

		m_xVal = x;
		m_yVal = y;
	}

	void getCurrentParameterNames(String &x, String &y)
	{
		if (!m_parameterVector.empty())
		{
			x = m_parameterVector[m_xParameter]->name;
			y = m_parameterVector[m_yParameter]->name;
		}
	}

	// Sets up which audio parameter is linked to which axis of the XY pad
	void setParameterMapping(int x, int y)
	{
		// Validate input
		if (x < 0 || x > m_parameterVector.size())
		{
			x = 0;
		}

		if (y < 0 || y > m_parameterVector.size())
		{
			y = 1;
		}

		m_xParameter = x;
		m_yParameter = y;
	}

	// Must specify effect type
	virtual EffectType getType() = 0;

	// AudioProcessor overrides

	const String getName() const override
	{
		return m_name;
	}

	bool acceptsMidi() const override
	{
		return false;
	}

	bool producesMidi() const override
	{
		return false;
	}

	AudioProcessorEditor* createEditor() override
	{
		return nullptr;
	}

	bool hasEditor() const override
	{
		return false;
	}

	int getNumPrograms() override
	{
		return 0;
	}

	int getCurrentProgram() override
	{
		return 0;
	}

	void setCurrentProgram(int index) override
	{
	}

	const String getProgramName(int index) override
	{
		return "";
	}

	void changeProgramName(int index, const String& newName) override
	{
	}
	void getStateInformation(juce::MemoryBlock& destData) override
	{
	}
	void setStateInformation(const void* data, int sizeInBytes) override
	{
	}

	void registerParameter(AudioParameterFloat* param)
	{
		m_parameterVector.push_back(param);
		//m_processor->registerParameter
	//	addParameter(m_parameterVector.back());
	}


	float getBaseXVal()
	{
		return m_xVal;
	}

	float getBaseYVal()
	{
		return m_yVal;
	}

	AudioProcessor* getProcessor()
	{
		return m_processor;
	}

	void setProcessor(AudioProcessor* mainProcessor)
	{
		m_processor = mainProcessor;
	}

	std::vector<AudioParameterFloat*> getAllParameters()
	{
		return m_parameterVector;
	}

private:

	std::vector<AudioParameterFloat*> m_parameterVector;
	bool m_active = false;
	String m_name;
	int m_xParameter, m_yParameter;
	float m_xVal, m_yVal;
	AudioProcessor* m_processor;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectBase)
};


