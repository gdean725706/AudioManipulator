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
	String m_labelText;
public:
    FlexButtonComponent(String buttonName, int width = 150, int height = 75, String labelText = "Record") :
		Button("btn" + buttonName),
		FlexItem(width, height),
		m_baseColour(getLookAndFeel().findColour(TextButton::buttonColourId)),
		m_colour(m_baseColour),
		m_active(false),
		m_labelText(labelText)
    {
		flexGrow = 1.0f;
		flexShrink = 1.0f;
		associatedComponent = this;
    }

    ~FlexButtonComponent()
    {
    }

	void setLabelText(String text)
	{
		m_labelText = text;
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

		g.setColour(Colours::lightgrey);
        float labelSize = scaleRange((float)getWidth(), 60.0f, 334.0f, 8.0f, 16.0f);
		g.setFont(labelSize);
		g.drawText(m_labelText, getLocalBounds(), Justification::centredTop, false);

		g.setColour(Colours::white);

		float fontSize = scaleRange((float)getWidth(), 60.0f, 334.0f, 16.0f, 64.0f);
		g.setFont(fontSize);
		

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

