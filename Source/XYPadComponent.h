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
		m_colour(Colours::lightgrey)
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

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("XYPadComponent", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
	Colour m_colour;
	float m_pointX, m_pointY;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};
