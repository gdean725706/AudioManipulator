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
    XYPadComponent(int width, int height):
		m_width(width),
		m_height(height),
		FlexItem(width,height),
		m_pointX(0),
		m_pointY(height),
		m_colour(Colours::lightgrey),
		m_currentXY("Default")
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		//Set up flex item
		associatedComponent = this;
		alignSelf = AlignSelf::autoAlign;
		order = 0;
    }

    ~XYPadComponent()
    {
    }

	// Getters for X & Y values
	// Return value normalised to a specified range
	int getXValue(int minScaleValue, int maxScaleValue)
	{
		return scaleRange(m_pointX, 0, m_width, minScaleValue, maxScaleValue);
	}
	float getXValueNormalised()
	{
		float v = (float)m_pointX / (float)m_width;
		return v;
	}

	int getYValue(int minScaleValue, int maxScaleValue)
	{
		int invertedY = m_height - m_pointY;
		return scaleRange(invertedY, 0, m_height, minScaleValue, maxScaleValue);
	}
	float getYValueNormalised()
	{
		// Need to invert the Y axis
		int invertedY = m_height - m_pointY;
		float v = (float)invertedY / (float)m_height;
		return v; 
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
		
		// Update string with XY coords
		updateMouseXYText();

		// Horizontal Line
		g.drawRect(m_pointX, 0, 2, this->getHeight(), 1);
		// Vertical line
		g.drawRect(0, m_pointY, this->getWidth(), 2, 1);

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

	// Update XY point when mouse clicked
	void mouseDown(const MouseEvent& event) override
	{
		updateXYPoints();

		repaint();
	}

	// Allow for dragging XY input
	void mouseDrag(const MouseEvent& event) override
	{
		updateXYPoints();

		repaint();
	}

	void updateXYPoints()
	{
		// get mouse point relative to this component
		auto mousePoint = this->getMouseXYRelative();

		// Clamp values so we can't exceed bounds of component
		m_pointX = clamp(mousePoint.x, m_width, (double)0.0f);
		m_pointY = clamp(mousePoint.y, m_height, (double) 0.0f);
	}

	// Update string containing mouse XY values
	void updateMouseXYText()
	{
		// Dynamic casting stream int to string
		String xText, yText;
		xText << m_pointX;
		yText << m_pointY;
		
		m_currentXY = xText + "," + yText;
	}

private:
	Colour m_colour;
	int m_width, m_height;
	int m_pointX, m_pointY;
	String m_currentXY;

	int scaleRange(int input, int inputStart, int inputEnd, int outputStart, int outputEnd)
	{
		return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
	}

	double clamp(double x, double upper, double lower)
	{
		return std::min(upper, std::max(x, lower));
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};
