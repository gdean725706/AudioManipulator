/*
  ==============================================================================

    EffectButtonComponent.h
    Created: 19 Oct 2017 10:30:23pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectBase.h"

class EffectButtonComponent : public Button, public FlexItem
{

private:
	Colour m_colour;
	int m_midiPitch;
	typedef EffectBase::EffectType EffectType;
	EffectType m_effectType;
	String m_labelText;
	bool m_active;
public:
	EffectButtonComponent(EffectType effectType, String buttonName) :
		m_labelText(buttonName),
		FlexItem(60, 40),
		Button("btnEffect" + buttonName),
		m_midiPitch(),
		m_colour(Colours::lightgrey),
		m_effectType(effectType),
		m_active(false)
	{
		// Randomize colour
		//Random r;
		//m_colour = Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));

		// Due to FlexBox multiple inheritance we must set the associated component to this
		associatedComponent = this;
		flexShrink = 1.0;
		flexGrow = 1.0;
	}

	// Override paint virtual to make it do what we want it to do
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
	{

		// Set up button colour to change based on mouse events
		if (isButtonDown) {
			m_colour = Colours::black;
		}
		else {
			m_colour = Colours::grey;
		}


		if (isMouseOverButton)
		{
			m_colour = m_colour.brighter();
		}

		if (m_active)
		{
			m_colour = Colours::darkgrey;
		}

		// fill colours
		g.fillAll(m_colour);

		g.setColour(Colours::white);
		float fontSize = scaleRange((float)getWidth(), 60.0f, 120.0f, 15.0f, 30.0f);
		g.setFont(fontSize);
		g.drawFittedText(m_labelText, getLocalBounds(), Justification::centred, 2);
	}

	int getPitch() 
	{
		return m_midiPitch;
	}

	void setActive(bool active)
	{
		m_active = active;
	}

	EffectType getType()
	{
		return m_effectType;
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectButtonComponent)
};
