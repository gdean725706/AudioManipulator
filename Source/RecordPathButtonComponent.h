/*
  ==============================================================================

    RecordPathButtonComponent.h
    Created: 16 Nov 2017 1:10:45am
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class RecordPathButtonComponent    : public Button, public FlexItem
{
private:
	Colour m_colour;
	bool m_active;
	String m_labelText;
public:
    RecordPathButtonComponent(String buttonName) :
		Button("btnPath" + buttonName),
		FlexItem(75, 50),
		m_labelText(buttonName),
		m_colour(Colours::lightgrey),
		m_active(false)
    {
		alignSelf = AlignSelf::autoAlign;
		associatedComponent = this;
    }

    ~RecordPathButtonComponent()
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
		g.drawText(m_labelText, getLocalBounds(), Justification::centred, false);
	}

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

	void setActive(bool active)
	{
		m_active = active;
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordPathButtonComponent)
};
