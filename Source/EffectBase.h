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

//Make this abstract

class EffectBase
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

	virtual void getType() = 0;

private:
	bool m_active = false;

	int m_Xparameter, m_yParameter;

	std::vector<AudioParameterFloat>  m_parameterVector;
};