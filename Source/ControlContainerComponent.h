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
#include "FlexContainer.h"
#include "FlexComboBox.h"

//==============================================================================
/*
*/
class ControlContainerComponent    : public FlexContainer,
	public SliderListener, public ButtonListener, public ComboBoxListener
{
public:
    ControlContainerComponent(MainAudioProcessor* p, int number):
		m_writingPoints(false),
		m_readingPoints(false),
		m_processor(p),
		m_chainNumber(number)
    {


		// Add XY pad and sliders
		m_XYPad = new XYPadComponent(300, 200, m_processor->getChain(m_chainNumber), p);
		// Add and setup sliders
		m_slider1 = new FlexSlider("flexSlider1", 10, 50);
		m_slider2 = new FlexSlider("flexSlider2", 10, 50);

		m_slider1->setRange(-1.0f, 1.0f, 0.01f);
		m_slider2->setRange(-1.0f, 1.0f, 0.01f);

		m_slider1->setSliderStyle(Slider::LinearVertical);
		m_slider2->setSliderStyle(Slider::LinearVertical);

		m_slider1->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
		m_slider2->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

		m_slider1->addListener(this);
		m_slider2->addListener(this);
		// Add and make visible
		//addAndMakeVisible(m_slider1);
		//addAndMakeVisible(m_slider2);
		addAndMakeVisible(m_XYPad);

		// Add to flex
		//items.add(m_slider1->withMargin(3));
		//items.add(m_slider2->withMargin(3));
		items.add(m_XYPad->withMargin(3));

		// Set up order
		m_slider1->order = 0;
		m_slider2->order = 1;
		m_XYPad->order = 2;

		m_slider1->flexGrow = 1.0f;
		m_slider2->flexGrow = 1.0f;

		m_rightContainer = new FlexContainer();
		addAndMakeVisible(m_rightContainer);
		items.add(m_rightContainer->withMargin(3));
		m_rightContainer->associatedComponent = m_rightContainer;

		for (int i = 0; i < 3; ++i)
		{
			m_buttonStates[i] = SlotState::Empty;
			m_pathButtons[i] = new FlexButtonComponent("RecordPaths" + i, 50, 25);
			m_pathButtons[i]->addListener(this);
			m_rightContainer->items.add(m_pathButtons[i]->withMargin(3));
			m_rightContainer->addAndMakeVisible(m_pathButtons[i]);
			m_pathButtons[i]->order = i + 3;
		}

		m_pathButtons[0]->setButtonText("1");
		m_pathButtons[1]->setButtonText("2");
		m_pathButtons[2]->setButtonText("3");

		// Playback rate slider
		m_playbackRateSlider = new FlexSlider("playbackRateSlider", 50, 25);
		m_playbackRateSlider->setRange(-2.5, 2.5, 0.01);
		m_playbackRateSlider->setSliderStyle(Slider::LinearHorizontal);
		m_playbackRateSlider->order = 5;
		m_playbackRateSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 50, 15);
		m_playbackRateSlider->setValue(1.0, NotificationType::dontSendNotification);
		m_rightContainer->items.add(m_playbackRateSlider->withMargin(3));
		m_playbackRateSlider->addListener(this);
		m_rightContainer->addAndMakeVisible(m_playbackRateSlider);

		// Interpolation selection box
		m_interpSelection = new FlexComboBox("interpolationSelectionBox");
		m_interpSelection->addItem("Cubic", 1);
		m_interpSelection->addItem("Linear", 2);
		m_interpSelection->setTextWhenNothingSelected("Smoothing Mode");
		m_rightContainer->items.add(m_interpSelection->withMargin(3));
		m_interpSelection->addListener(this);
		m_rightContainer->addAndMakeVisible(m_interpSelection);
		m_rightContainer->performLayout(m_rightContainer->getLocalBounds());
		m_interpSelection->flexGrow = 1.0f;
		m_interpSelection->flexShrink = 2.0f;

		// flexWrap CSS equiv attribute
		flexDirection = Direction::row;
		flexWrap = Wrap::noWrap;
		justifyContent = JustifyContent::center;
		alignItems = AlignItems::center;
		alignContent = AlignContent::stretch;

		// flexWrap CSS equiv attribute
		m_rightContainer->flexDirection = Direction::column;
		m_rightContainer->flexWrap = Wrap::noWrap;
		m_rightContainer->justifyContent = JustifyContent::center;
		m_rightContainer->alignItems = AlignItems::center;
		m_rightContainer->alignContent = AlignContent::stretch;

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

		int rightContainerWidth = getWidth() * 0.25f;
		m_rightContainer->setBounds(bounds.removeFromRight(rightContainerWidth).reduced(3));
		m_rightContainer->width = rightContainerWidth;

		int buttonWidth = m_rightContainer->getWidth() * 0.9f;
		int rightContainerHeight = m_rightContainer->getHeight();

		for (int i = 0; i < 3; ++i)
		{
			m_pathButtons[i]->width = buttonWidth;
			m_pathButtons[i]->height = rightContainerHeight * 0.33f;
		}
		m_rightContainer->performLayout(m_rightContainer->getLocalBounds());
		performLayout(bounds);
		
    }

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == m_playbackRateSlider)
		{
			m_XYPad->setPlaybackRate(m_playbackRateSlider->getValue());
		}
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

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged)
	{
		if (comboBoxThatHasChanged == m_interpSelection)
		{
			// Selection IDs in interp combo box correspond to XY interp mode enum, so can cast
			m_XYPad->setInterpolationMode((XYPadComponent::InterpolationMode)m_interpSelection->getSelectedId());
		}
	}

	// Forces XY to update.
	// x and y must be normalised values (0.0-1.0)
	void updateXYGUI(float x, float y)
	{
		m_XYPad->forceXYUpdate(x, y);
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
			if (!m_writingPoints && !m_readingPoints)
			{
				m_XYPad->startRecordingPoints(index);
				flexBtn->updateBaseColour(Colours::red);
				m_buttonStates[index] = SlotState::Recording;
				m_writingPoints = true;
			}
			break;
		}
		case SlotState::Recording:
		{
			m_XYPad->stopRecordingPoints(index);
			flexBtn->updateBaseColour(Colours::green);
			m_buttonStates[index] = SlotState::Ready;
			m_writingPoints = false;
			break;
		}
		case SlotState::Ready:
		{
			m_XYPad->startPointPlayback(index);
			flexBtn->updateBaseColour(Colours::lightgreen);
			m_buttonStates[index] = SlotState::Playback;
			m_readingPoints = true;
			break;
		}
		case SlotState::Playback:
		{
			m_XYPad->stopPointPlayback(index);
			flexBtn->updateBaseColour(Colours::green);
			m_buttonStates[index] = SlotState::Ready;
			m_readingPoints = false;
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

	typedef ScopedPointer<XYPadComponent> XYPadPtr;
	XYPadPtr m_XYPad;
	ScopedPointer<FlexSlider> m_slider1, m_slider2, m_playbackRateSlider;

	ScopedPointer<FlexContainer> m_rightContainer;

	ScopedPointer<FlexComboBox> m_interpSelection;

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
