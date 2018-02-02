/*
  ==============================================================================

    BottomContainer.h
    Created: 6 Dec 2017 5:01:50pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"
#include "FlexButtonComponent.h"
#include <vector>

//==============================================================================
/*
*/
class BottomContainer    : public Component, public FlexBox, public ButtonListener
{
public:
    BottomContainer(MainAudioProcessor *p):
		m_processor(p)
    {
		for (int i = 0; i < 3; ++i)
		{
			m_writing[i] = false;
			m_recordButtons[i] = new FlexButtonComponent("RecordAudio" + i);
			m_recordButtons[i]->addListener(this);
			addAndMakeVisible(m_recordButtons[i]);
			items.add(m_recordButtons[i]->withMargin(3));
		}
		
		m_recordButtons[0]->setButtonText("A");
		m_recordButtons[1]->setButtonText("B");
		m_recordButtons[2]->setButtonText("C");

		flexDirection = Direction::row;
		flexWrap = Wrap::wrap;
		justifyContent = JustifyContent::center;
		alignItems = AlignItems::center;
		alignContent = AlignContent::spaceBetween;
    }

    ~BottomContainer()
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

        //g.setColour (Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("BottomContainer", getLocalBounds(),
        //            Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
		auto bounds = getLocalBounds();

		performLayout(bounds);
        // This method is where you should set the bounds of any child
        // components that your component contains..

	}

	// Could make it add recording slot?
	void buttonClicked(Button* button) override
	{
		if (button == m_recordButtons[0])
		{
			toggleButton(0);
		}
		else if (button == m_recordButtons[1])
		{
			toggleButton(1);
		}
		else if (button == m_recordButtons[2])
		{
			toggleButton(2);
		}

	}

private:

	void toggleButton(int index)
	{
		if (index < 0 || index > 3) return;
		m_writing[index] = !m_writing[index];
		if (m_writing[index])
		{
			m_processor->startRecording(index);
		}
		else
		{
			m_processor->stopRecording(index);
		}

		m_recordButtons[index]->setActive(m_writing[index]);
	}

	int m_width, m_height;
	MainAudioProcessor* m_processor;
	typedef ScopedPointer<FlexButtonComponent> FlexButtonPtr;
	
	FlexButtonPtr m_recordButtons[3];
	bool m_writing[3];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomContainer)
};
