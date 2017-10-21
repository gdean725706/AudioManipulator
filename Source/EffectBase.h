/*
  ==============================================================================

    EffectBase.h
    Created: 21 Oct 2017 4:02:47pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// In good practise would this be a struct instead?
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
private:
	bool m_active = false;
};