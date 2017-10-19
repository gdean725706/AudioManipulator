/*
  ==============================================================================

    EffectButtonContainerComponent.h
    Created: 19 Oct 2017 11:17:41pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class EffectButtonContainerComponent    : public Component, public FlexBox
{
public:
    EffectButtonContainerComponent(int width, int height):
		m_width(width),
		m_height(height)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		// Add buttons
		m_button1 = new EffectButtonComponent(EffectButtonComponent::EffectType::LowPassFilter, "LPF");
		m_button2 = new EffectButtonComponent(EffectButtonComponent::EffectType::HighPassFilter, "HPF");
		m_button3 = new EffectButtonComponent(EffectButtonComponent::EffectType::Delay, "Delay");

		// Make visible
		addAndMakeVisible(m_button1);
		addAndMakeVisible(m_button2);
		addAndMakeVisible(m_button3);

		//Add to flexbox
		items.add(m_button1->withMargin(3));
		items.add(m_button2->withMargin(3));
		items.add(m_button3->withMargin(3));

        // Set up flexbox
	    flexDirection = Direction::row;
	    flexWrap = Wrap::wrap;
	    justifyContent = JustifyContent::spaceAround;
	    alignItems = AlignItems::center;
	    alignContent = AlignContent::spaceAround;

    }

    ~EffectButtonContainerComponent()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (Colours::lightcoral);   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("EffectButtonContainerComponent", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
	int m_width, m_height;
	typedef ScopedPointer<EffectButtonComponent> FlexButtonPtr;
	FlexButtonPtr m_button1, m_button2, m_button3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectButtonContainerComponent)
};
