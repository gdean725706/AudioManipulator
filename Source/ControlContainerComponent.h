/*
  ==============================================================================

    ControlContainerComponent.h
    Created: 24 Oct 2017 7:20:00pm
    Author:  deano

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ControlContainerComponent    : public Component, public FlexBox
{
public:
    ControlContainerComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
		m_XYPad = new XYPadComponent(400, 400);

		addAndMakeVisible(m_XYPad);

		items.add(m_XYPad->withMargin(3));

		// flexWrap CSS equiv attribute
		flexDirection = Direction::row;
		flexWrap = Wrap::wrap;
		justifyContent = JustifyContent::center;
		alignItems = AlignItems::stretch;
		alignContent = AlignContent::spaceBetween;

    }

    ~ControlContainerComponent()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("ControlContainerComponent", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
		performLayout(getLocalBounds());
    }

	ScopedPointer<XYPadComponent> getXYPad()
	{
		return m_XYPad;
	}

private:
	typedef ScopedPointer<XYPadComponent> XYPadPtr;
	XYPadPtr m_XYPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlContainerComponent)
};
