/*
  ==============================================================================

    ControlContainerComponent.h
    Created: 24 Oct 2017 7:20:00pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FlexSlider.h"
#include "RecordPathButtonComponent.h"
#include "XYPadComponent.h"
#include "EffectChain.h"
//==============================================================================
/*
*/
class ControlContainerComponent    : public Component, public FlexBox,
	public SliderListener, public ButtonListener
{
public:
    ControlContainerComponent(int number, MainAudioProcessor& p):
		m_writingPoints(false),
		m_processor(p),
		m_chainNumber(number)
    {
		// Buttons
		m_pathButton1 = new RecordPathButtonComponent("Path1");
		m_testButton1 = new TextButton("Test");

		// Set this effect chain
		m_processor.getChain(&m_effectChain, m_chainNumber);

		// Add XY pad and sliders
		m_XYPad = new XYPadComponent(400, 400, m_effectChain, p);

		// Add and setup sliders
		m_slider1 = new FlexSlider("flexSlider1");
		m_slider2 = new FlexSlider("flexSlider2");

		m_slider1->setRange(0, 1);
		m_slider2->setRange(0, 1);

		m_slider1->setSliderStyle(Slider::LinearVertical);
		m_slider2->setSliderStyle(Slider::LinearVertical);

		m_slider1->addListener(this);
		m_slider2->addListener(this);

		m_pathButton1->addListener(this);
		m_testButton1->addListener(this);

		// Add and make visible
		addAndMakeVisible(m_XYPad);
		addAndMakeVisible(m_slider1);
		addAndMakeVisible(m_slider2);
		addAndMakeVisible(m_pathButton1);
		addAndMakeVisible(m_testButton1);

		// Add to flex
		items.add(m_XYPad->withMargin(3));
		items.add(m_slider1->withMargin(3));
		items.add(m_slider2->withMargin(3));
		items.add(m_pathButton1->withMargin(3));

		// Set up order
		m_slider1->order = 0;
		m_slider2->order = 1;
		m_pathButton1->order = 3;
		m_XYPad->order = 2;

		// flexWrap CSS equiv attribute
		flexDirection = Direction::row;
		flexWrap = Wrap::wrap;
		justifyContent = JustifyContent::center;
		alignItems = AlignItems::center;
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

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		// Call flexbox performlayout method to arrange flex items
		auto bounds = getLocalBounds();

		performLayout(bounds);
    }

	void sliderValueChanged(Slider* slider) override
	{

	}

	void buttonClicked(Button* button) override
	{
		if (button == m_pathButton1)
		{
			// Toggle writing points
			m_writingPoints = !m_writingPoints;
			m_pathButton1->setActive(m_writingPoints);
			m_XYPad->writePoints(m_writingPoints);
		}
	}

private:
	ScopedPointer<RecordPathButtonComponent> m_pathButton1;
	ScopedPointer<TextButton> m_testButton1;
	typedef ScopedPointer<XYPadComponent> XYPadPtr;
	XYPadPtr m_XYPad;
	ScopedPointer<FlexSlider> m_slider1, m_slider2;

	bool m_writingPoints;
	// Reference to linked effect chain
	MainAudioProcessor& m_processor;
	EffectChain m_effectChain;
	int m_chainNumber;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlContainerComponent)
};
