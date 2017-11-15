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

class EffectBase : public AudioProcessor
{
public:
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
		return false;
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
protected:

	std::vector<AudioParameterFloat*>  m_parameterVector;

private:
	bool m_active = false;
	String m_name;
	int m_Xparameter, m_yParameter;


};