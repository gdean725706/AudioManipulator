/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYPadComponent.h"
#include "EffectButtonComponent.h"
#include "EffectButtonContainerComponent.h"
#include "ControlContainerComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
// Use multiple inheritance to inherit flexbox into our component

class MainContentComponent   : public AudioAppComponent, public FlexBox, public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
    void paint (Graphics&) override;
    void resized() override;
	void mouseMove(const MouseEvent& event) override;

	void buttonClicked(Button* button) override;

private:
	ScopedPointer<TextButton> m_settingsButton;

	// Typedef for our enum containing all the FX types
	typedef EffectBase::EffectType FXType;

	// Create container for our flex buttons
	ScopedPointer<EffectButtonContainerComponent> m_effectButtonContainer;
	// Create container for control buttons
	ScopedPointer<ControlContainerComponent> m_controlsContainer;

	ScopedPointer<XYPadComponent> m_XYPad1;

	// Define midi output device
	MidiOutput* m_midiOutput;

	// Define a fiter object
	ScopedPointer<IIRFilter> m_testFilter;
	double m_filterFreq, m_filterRes;

	double m_sampleRate;

	void playNote(int note);

	double scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)

};
