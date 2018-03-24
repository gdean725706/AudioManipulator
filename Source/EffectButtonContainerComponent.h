/*
  ==============================================================================

    EffectButtonContainerComponent.h
    Created: 19 Oct 2017 11:17:41pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectButtonComponent.h"
#include "MainProcessor.h"
#include "FlexContainer.h"

//==============================================================================
/*
*/
class EffectButtonContainerComponent    : public FlexContainer, public ButtonListener
{
public:
	EffectButtonContainerComponent(MainAudioProcessor* p) :
		m_processor(p)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		// Add buttons
		m_buttonLPF = new EffectButtonComponent(EffectType::LowPassFilter, "LPF");
		m_buttonHPF = new EffectButtonComponent(EffectType::HighPassFilter, "HPF");
		m_buttonDelay = new EffectButtonComponent(EffectType::Delay, "Delay");
		m_buttonChorus = new EffectButtonComponent(EffectType::Chorus, "Chorus");
		m_buttonFlanger = new EffectButtonComponent(EffectType::Flanger, "Flanger");
		m_buttonPlaybackSpeed = new EffectButtonComponent(EffectType::Speed, "Speed");
		m_buttonPitchShifter = new EffectButtonComponent(EffectType::PitchShifter, "Pitch Shifter");
		m_buttonRingMod = new EffectButtonComponent(EffectType::RingMod, "Ring Mod");

		// LPF init state on
		m_buttonLPF->setActive(true);

		// Make visible
		addAndMakeVisible(m_buttonLPF);
		addAndMakeVisible(m_buttonHPF);
		addAndMakeVisible(m_buttonDelay);
		addAndMakeVisible(m_buttonChorus);
		addAndMakeVisible(m_buttonFlanger);
		addAndMakeVisible(m_buttonPlaybackSpeed);
		addAndMakeVisible(m_buttonPitchShifter);
		addAndMakeVisible(m_buttonRingMod);

		//Add to flexbox
		items.add(m_buttonLPF->withMargin(3));
		items.add(m_buttonHPF->withMargin(3));
		items.add(m_buttonDelay->withMargin(3));
		items.add(m_buttonChorus->withMargin(3));
		items.add(m_buttonFlanger->withMargin(3));
		items.add(m_buttonPlaybackSpeed->withMargin(3));
		items.add(m_buttonPitchShifter->withMargin(3));
		items.add(m_buttonRingMod->withMargin(3));

		//
		m_buttonLPF->addListener(this);
		m_buttonHPF->addListener(this);
		m_buttonDelay->addListener(this);
		m_buttonChorus->addListener(this);
		m_buttonFlanger->addListener(this);
		m_buttonPlaybackSpeed->addListener(this);
		m_buttonPitchShifter->addListener(this);
		m_buttonRingMod->addListener(this);

        // Set up flexbox
	    flexDirection = Direction::row;
	    flexWrap = Wrap::wrap;
	    justifyContent = JustifyContent::center;
	    alignItems = AlignItems::center;
	    alignContent = AlignContent::stretch;

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

        g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId).brighter());   // clear the background

        g.setColour (Colours::transparentWhite);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("EffectButtonContainerComponent", getLocalBounds(),
        //            Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		auto bounds = getLocalBounds();
		performLayout(bounds);

    }

	void buttonClicked(Button* button) override
	{
		EffectButtonComponent* effectButton = dynamic_cast<EffectButtonComponent*>(button);

		m_currentEffect = effectButton->getType();

		// Update active buttons
		m_buttonLPF->setActive(m_currentEffect == EffectType::LowPassFilter);
		m_buttonHPF->setActive(m_currentEffect == EffectType::HighPassFilter);
		m_buttonDelay->setActive(m_currentEffect == EffectType::Delay);
		m_buttonChorus->setActive(m_currentEffect == EffectType::Chorus);
		m_buttonFlanger->setActive(m_currentEffect == EffectType::Flanger);
		m_buttonPlaybackSpeed->setActive(m_currentEffect == EffectType::Speed);
		m_buttonPitchShifter->setActive(m_currentEffect == EffectType::PitchShifter);
		m_buttonRingMod->setActive(m_currentEffect == EffectType::RingMod);

		// Update AudioProcessor
		m_processor->setCurrentEffect(m_currentEffect);

		repaint();

	}

	EffectBase::EffectType getCurrentEffect()
	{
		return m_currentEffect;
	}

private:
	typedef ScopedPointer<EffectButtonComponent> FlexButtonPtr;
	FlexButtonPtr m_buttonLPF, m_buttonHPF, m_buttonDelay, m_buttonChorus, m_buttonFlanger,
		m_buttonPlaybackSpeed, m_buttonPitchShifter, m_buttonRingMod;
	typedef EffectBase::EffectType EffectType;
	EffectType m_currentEffect;
	MainAudioProcessor* m_processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectButtonContainerComponent)
};
