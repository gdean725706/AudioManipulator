/*
  ==============================================================================

    ModContainer.h
    Created: 26 Mar 2018 8:41:15pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FlexContainer.h"
#include "XYPadComponent.h"
#include "FlexSlider.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class ModContainer    : public FlexContainer, public Slider::Listener
{
public:
    ModContainer(XYPadComponent* xy, int id, float lfoMaxRate = 10.0f):
		m_XYPad(xy),
		m_id(id),
		m_lfoMaxRate(lfoMaxRate)
    {
		m_sliders.reserve(3);
		setName("modContainer_" + (String)m_id);

		associatedComponent = this;
		flexGrow = 1.0f;
		flexShrink = 1.0f;

		m_LFORateSlider = new FlexSlider("LFORateSlider" + (String)id, 57, 57);
		m_ampXSlider = new FlexSlider("AmpXSlider" + (String)id, 52, 52);
		m_ampYSlider = new FlexSlider("AmpYSlider" + (String)id, 52 ,52);

		m_laf1.setCustomText("LFO " + (String)(id+1) + " Rate");
		m_laf2.setCustomText("X");
		m_laf3.setCustomText("Y");

		m_laf2.setJustification(Justification::left);
		m_laf3.setJustification(Justification::left);

		m_LFORateSlider->setLookAndFeel(&m_laf1);
		m_ampXSlider->setLookAndFeel(&m_laf2);
		m_ampYSlider->setLookAndFeel(&m_laf3);

		//m_rateLabel = new Label("LFORateLabel_" + (String)id, "LFO Rate " + (String)id);
		//m_ampXLabel = new Label("AmpXLabel_" + (String)id, "Mod X " + (String)id);
		//m_ampYLabel = new Label("AmpYLabel_" + (String)id, "Mod Y " + (String)id);

		//m_rateLabel->attachToComponent(m_LFORateSlider, false);
		//m_ampXLabel->attachToComponent(m_ampXSlider, false);
		//m_ampYLabel->attachToComponent(m_ampYSlider, false);

		//m_rateLabel->setColour(Label::textColourId, Colours::lightgrey);
		//m_ampXLabel->setColour(Label::textColourId, Colours::lightgrey);
		//m_ampYLabel->setColour(Label::textColourId, Colours::lightgrey);

		//m_rateLabel->setJustificationType(Justification::centredTop);
		//m_ampXLabel->setJustificationType(Justification::centredTop);
		//m_ampYLabel->setJustificationType(Justification::centredTop);

		m_sliders.push_back(m_LFORateSlider);
		m_sliders.push_back(m_ampXSlider);
		m_sliders.push_back(m_ampYSlider);


		m_LFORateSlider->flexGrow = 2.0f;
		m_LFORateSlider->flexShrink = 2.0f;
		m_ampXSlider->flexGrow = 1.0f;
		m_ampXSlider->flexShrink = 1.0f;
		m_ampYSlider->flexGrow = 1.0f;
		m_ampYSlider->flexShrink = 1.0f;

		m_LFORateSlider->setRange(0.0, m_lfoMaxRate, 0.02);
		m_ampXSlider->setRange(0.0, 1.0, 0.01);
		m_ampYSlider->setRange(0.0, 1.0, 0.01);

		m_LFORateSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		m_ampXSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		m_ampYSlider->setSliderStyle(Slider::RotaryVerticalDrag);

		m_LFORateSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 48, 15);
		m_ampXSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,true,40, 15);
		m_ampYSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true,40, 15);

		m_LFORateSlider->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		m_ampXSlider->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		m_ampYSlider->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);

		m_LFORateSlider->setTextValueSuffix("Hz");

		m_LFORateSlider->addListener(this);
		m_ampXSlider->addListener(this);
		m_ampYSlider->addListener(this);


		items.add(m_ampXSlider->withMargin(0));
		items.add(m_ampYSlider->withMargin(0));
		items.add(m_LFORateSlider->withMargin(0));

		addAndMakeVisible(m_LFORateSlider);
		addAndMakeVisible(m_ampXSlider);
		addAndMakeVisible(m_ampYSlider);

		flexDirection = Direction::column;
		flexWrap = Wrap::wrap;
		justifyContent = JustifyContent::spaceBetween;
		alignItems = AlignItems::flexStart;
		alignContent = AlignContent::stretch;

    }

    ~ModContainer()
    {
	}

	void setLFOMaxRate(float rate)
	{
		m_lfoMaxRate = rate;
		m_LFORateSlider->setRange(0.0, m_lfoMaxRate, 0.2);
	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == m_LFORateSlider)
		{
			m_XYPad->setLfoFrequency(m_LFORateSlider->getValue(), m_id);
		}
		if (slider == m_ampXSlider)
		{
			m_XYPad->setLfoAmplitude(m_ampXSlider->getValue(), m_id, 0);
		}
		if (slider == m_ampYSlider)
		{
			m_XYPad->setLfoAmplitude(m_ampYSlider->getValue(), m_id, 1);
		}
	}

	void paint (Graphics& g) override
	{
	    /* This demo code just fills the component's background and
	       draws some placeholder text to get you started.

	       You should replace everything in this method with your own
	       drawing code..
	    */

	    //g.fillAll (getLookAndFeel().findColour(TextButton::buttonColourId).brighter());   // clear the background

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

	std::vector<FlexSlider*> getSliders()
	{
		return m_sliders;
	}

private:

	ScopedPointer<FlexSlider> m_LFORateSlider, m_ampXSlider, m_ampYSlider;
	std::vector<FlexSlider*> m_sliders;
	XYPadComponent* m_XYPad;
	int m_id;
	float m_lfoMaxRate;
	ScopedPointer<Label> m_rateLabel, m_ampXLabel, m_ampYLabel;
	CustomLookAndFeel m_laf1, m_laf2, m_laf3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModContainer)
};
