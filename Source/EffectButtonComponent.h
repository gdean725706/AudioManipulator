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

class EffectButtonComponent : public Button, public FlexItem, public EffectBase
{

private:
	Colour m_colour;
	int m_midiPitch;

	EffectType m_effectType;
	String m_labelText;

public:
	EffectButtonComponent(EffectType effectType, String labelText) :
		m_effectType(effectType),
		m_labelText(labelText),
		FlexItem(75, 50),
		Button("Effect Button"),
		m_midiPitch(),
		m_colour(Colours::lightgrey)
	{
		// Randomize colour
		//Random r;
		//m_colour = Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));

		// Due to FlexBox multiple inheritance we must set the associated component to this
		associatedComponent = this;
		alignSelf = AlignSelf::autoAlign;

		// Not active yet
		setActive(true);
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

		if (isActive())
		{
			m_colour = m_colour.brighter();
		}

		// fill colours
		g.fillAll(m_colour);

		g.setColour(Colours::white);
		g.drawText(m_labelText, getLocalBounds(), Justification::centred, false);
	}

	int getPitch() 
	{
		return m_midiPitch;
	}
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectButtonComponent)
};
