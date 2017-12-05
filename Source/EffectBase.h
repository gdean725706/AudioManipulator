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

class EffectBase : public AudioProcessor
{
public:
	EffectBase() :
		m_xParameter(0),
		m_yParameter(1),
		m_xVal(0),
		m_yVal(0)
	{}
	enum EffectType
	{
		LowPassFilter,
		HighPassFilter,
		Phaser,
		Delay
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
			x = scaleRange(x, 0, 1, m_parameterVector[m_xParameter]->range.start, m_parameterVector[m_xParameter]->range.end);
			y = scaleRange(y, 0, 1, m_parameterVector[m_yParameter]->range.start, m_parameterVector[m_yParameter]->range.end);

			*m_parameterVector[m_xParameter] = x;
			*m_parameterVector[m_yParameter] = y;
		}

		m_xVal = x;
		m_yVal = y;
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

	// push our audioprocessor parameters to the vector
	void registerAudioParameters(const OwnedArray<AudioProcessorParameter>& params)
	{
		for (auto param : params)
		{
			m_parameterVector.push_back(dynamic_cast<AudioParameterFloat*>(param));
		}
	}

	// Scales a value from one range to another
	float scaleRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd)
	{
		return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart); 
	}

protected:
	std::vector<AudioParameterFloat*> m_parameterVector;

private:
	bool m_active = false;
	String m_name;
	int m_xParameter, m_yParameter;
	float m_xVal, m_yVal;


};