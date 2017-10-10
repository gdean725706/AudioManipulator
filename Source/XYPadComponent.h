/*
  ==============================================================================

    XYPadComponent.h
    Created: 10 Oct 2017 1:11:23pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class XYPadComponent    : public Component, public FlexItem
{
public:
    XYPadComponent():
		FlexItem(400,400),
		m_pointX(0),
		m_pointY(0),
		m_colour(Colours::lightgrey),
		m_currentXY("Default")
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		//Set up flexbox
		associatedComponent = this;
		alignSelf = AlignSelf::center;
    }

    ~XYPadComponent()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */


        g.fillAll (m_colour);   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
		
		// Get mouse X & Y pos
		int mouseX, mouseY;
		mouseX = this->getMouseXYRelative().x;
		mouseY = this->getMouseXYRelative().y;
		// Get component width & height
		int height = this->getHeight();
		int width = this->getWidth();

		// Horizontal Line
		g.drawRect(mouseX, 0, 2, this->getHeight(), 1);
		// Vertical line
		g.drawRect(0, mouseY, this->getWidth(), 2, 1);

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText (m_currentXY, getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
   
	}

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

	void mouseMove(const MouseEvent& event) override
	{
		// get mouse point relative to this component
		auto mousePoint = this->getMouseXYRelative();
		// Dynamic casting stream int to string
		String xText, yText;
		xText << mousePoint.x;
		yText << mousePoint.y;

		m_currentXY = xText + "," + yText;
		
		repaint();
	}

	void mouseDown(const MouseEvent& event) override
	{
		m_pointX = this->getMouseXYRelative().x;
		m_pointY = this->getMouseXYRelative().y;
	}


private:
	Colour m_colour;
	int m_pointX, m_pointY;
	String m_currentXY;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};
