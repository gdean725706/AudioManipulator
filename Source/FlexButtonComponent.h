/*
  ==============================================================================

    FlexButtonComponent.h
    Created: 16 Nov 2017 1:10:45am
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

//abstract intermediary class with enum for each type
// inherit from
// istype

class FlexButtonComponent    : public Button, public FlexItem
{
private:
	Colour m_baseColour, m_colour;
	bool m_active;
public:
    FlexButtonComponent(String buttonName, int width = 150, int height = 75) :
		Button("btn" + buttonName),
		FlexItem(width, height),
		m_baseColour(getLookAndFeel().findColour(TextButton::buttonColourId)),
		m_colour(m_baseColour),
		m_active(false)
    {
		associatedComponent = this;
    }

    ~FlexButtonComponent()
    {
    }

	// Override paint virtual to make it do what we want it to do
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
	{

		// Set up button colour to change based on mouse events
		if (isButtonDown) {
			m_colour = Colours::black;
		}
		else {
			m_colour = m_baseColour;
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
		g.setFont(48);
		
		g.drawText(getButtonText(), getLocalBounds(), Justification::centred, false);
	}

	void setActive(bool active)
	{
		m_active = active;
	}

	bool isActive()
	{
		return m_active;
	}
	void updateBaseColour(Colour col)
	{
		m_baseColour = col;
	}
	void resetBaseColour()
	{
		m_baseColour = getLookAndFeel().findColour(TextButton::buttonColourId);
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexButtonComponent)
};

