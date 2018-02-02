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
    ControlContainerComponent(MainAudioProcessor* p, int number):
		m_writingPoints(false),
		m_readingPoints(false),
		m_writingAudio(false),
		m_processor(p),
		m_chainNumber(number)
    {
		// Buttons
		m_pathButton1 = new FlexButtonComponent("Path1");
		m_recordControlButton = new FlexButtonComponent("PlayPath1");
		m_recordAudioButton = new FlexButtonComponent("RecordAudio");

		// Add XY pad and sliders
		m_XYPad = new XYPadComponent(400, 400, m_processor->getChain(m_chainNumber), p);

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
		m_recordControlButton->addListener(this);
		m_recordAudioButton->addListener(this);

		// Add and make visible
		addAndMakeVisible(m_XYPad);
		//addAndMakeVisible(m_slider1);
		//addAndMakeVisible(m_slider2);
		addAndMakeVisible(m_pathButton1);
		addAndMakeVisible(m_recordControlButton);
		addAndMakeVisible(m_recordAudioButton);

		// Add to flex
		items.add(m_XYPad->withMargin(3));
		items.add(m_slider1->withMargin(3));
		items.add(m_slider2->withMargin(3));
		items.add(m_pathButton1->withMargin(3));
		items.add(m_recordControlButton->withMargin(3));
		items.add(m_recordAudioButton->withMargin(3));

		// Set up order
		m_slider1->order = 0;
		m_slider2->order = 1;
		m_XYPad->order = 2;
		m_pathButton1->order = 3;
		m_recordControlButton->order = 4;
		m_recordAudioButton->order = 5;

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
			if (!m_readingPoints)
			{
				// Toggle writing points
				m_writingPoints = !m_writingPoints;
				m_pathButton1->setActive(m_writingPoints);
				m_XYPad->writePoints(m_writingPoints);
			}
		}
		else if (button == m_recordControlButton)
		{
			if (!m_writingPoints)
			{
				m_readingPoints = !m_readingPoints;
				m_recordControlButton->setActive(m_readingPoints);
				if (m_readingPoints && !m_writingPoints)
				{
					m_XYPad->startPointPlayback();
				}
				else if (!m_writingPoints)
				{
					m_XYPad->stopPointPlayback();
				}
			}
		}
		else if (button == m_recordAudioButton)
		{
			m_writingAudio = !m_writingAudio;

			if (m_writingAudio)
			{
				m_processor->startRecording();
			}
			else
			{
				m_processor->stopRecording();
			}

			m_recordAudioButton->setActive(m_writingAudio);
		}
	}

private:
	ScopedPointer<FlexButtonComponent> m_pathButton1;
	ScopedPointer<FlexButtonComponent> m_recordControlButton;
	typedef ScopedPointer<XYPadComponent> XYPadPtr;
	XYPadPtr m_XYPad;
	ScopedPointer<FlexSlider> m_slider1, m_slider2;

	ScopedPointer<FlexButtonComponent> m_recordAudioButton;

	bool m_writingPoints, m_readingPoints, m_writingAudio;
	// Reference to linked effect chain
	MainAudioProcessor* m_processor;
	int m_chainNumber;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlContainerComponent)
};
