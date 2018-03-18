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
#include "FlexButtonComponent.h"
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
		m_processor(p),
		m_chainNumber(number)
    {
		for (int i = 0; i < 3; ++i)
		{
			m_buttonStates[i] = SlotState::Empty;
			m_pathButtons[i] = new FlexButtonComponent("RecordPaths" + i);
			m_pathButtons[i]->addListener(this);
			addAndMakeVisible(m_pathButtons[i]);
			items.add(m_pathButtons[i]->withMargin(3));
			m_pathButtons[i]->order = i + 4;
		}
		m_pathButtons[0]->setButtonText("1");
		m_pathButtons[1]->setButtonText("2");
		m_pathButtons[2]->setButtonText("3");

		// Buttons
		m_pathButton1 = new FlexButtonComponent("Path1");
		m_recordControlButton = new FlexButtonComponent("PlayPath1");

		m_pathButton1->setButtonText("Rec Path");
		m_recordControlButton->setButtonText("Play Path");

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

		// Add and make visible
		addAndMakeVisible(m_XYPad);
		//addAndMakeVisible(m_slider1);
		//addAndMakeVisible(m_slider2);
		//addAndMakeVisible(m_pathButton1);
		//addAndMakeVisible(m_recordControlButton);

		// Add to flex
		items.add(m_XYPad->withMargin(3));
		items.add(m_slider1->withMargin(3));
		items.add(m_slider2->withMargin(3));
		items.add(m_pathButton1->withMargin(3));
		items.add(m_recordControlButton->withMargin(3));

		// Set up order
		m_slider1->order = 0;
		m_slider2->order = 1;
		m_XYPad->order = 0;
		m_pathButton1->order = 3;
		m_recordControlButton->order = 4;

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
		if (button == m_pathButtons[0])
		{
			toggleRecordButton(button, 0);
		}
		if (button == m_pathButtons[1])
		{
			toggleRecordButton(button, 1);
		}
		if (button == m_pathButtons[2])
		{
			toggleRecordButton(button, 2);
		}
	}

private:
	void toggleRecordButton(Button* button, int index)
	{
		if (index < 0 || index > 3) return;
		FlexButtonComponent* flexBtn = dynamic_cast<FlexButtonComponent*>(button);

		switch (m_buttonStates[index])
		{
		case SlotState::Empty:
		{
			m_XYPad->startRecordingPoints(index);
			flexBtn->updateBaseColour(Colours::red);
			m_buttonStates[index] = SlotState::Recording;
			break;
		}
		case SlotState::Recording:
		{
			m_XYPad->stopRecordingPoints(index);
			flexBtn->updateBaseColour(Colours::green);
			m_buttonStates[index] = SlotState::Ready;
			break;
		}
		case SlotState::Ready:
		{
			m_XYPad->startPointPlayback(index);
			flexBtn->updateBaseColour(Colours::lightgreen);
			m_buttonStates[index] = SlotState::Playback;
			break;
		}
		case SlotState::Playback:
		{
			m_XYPad->stopPointPlayback(index);
			flexBtn->updateBaseColour(Colours::green);
			m_buttonStates[index] = SlotState::Ready;
			break;
		}
		case SlotState::Deleting:
		{
			m_buttonStates[index] = SlotState::Empty;
			break;
		}
		default:
			break;
		}

	}
	typedef ScopedPointer<FlexButtonComponent> FlexButtonPtr;
	FlexButtonPtr m_pathButtons[3];
	ScopedPointer<FlexButtonComponent> m_pathButton1;
	ScopedPointer<FlexButtonComponent> m_recordControlButton;

	typedef ScopedPointer<XYPadComponent> XYPadPtr;
	XYPadPtr m_XYPad;
	ScopedPointer<FlexSlider> m_slider1, m_slider2;

	enum SlotState
	{
		Empty,
		Recording,
		Ready,
		Playback,
		Deleting
	};

	SlotState m_buttonStates[3];

	bool m_writingPoints, m_readingPoints;
	// Reference to linked effect chain
	MainAudioProcessor* m_processor;
	int m_chainNumber;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlContainerComponent)
};
