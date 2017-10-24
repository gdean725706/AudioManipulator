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
	// Define our own scopedpointer custom component
	typedef ScopedPointer<XYPadComponent> XYPadPtr;

	typedef EffectBase::EffectType FXType;

	// Our test objects
	//TestPtr m_custom1, m_custom2, m_custom3, m_custom4, m_custom5;

	// Create XY Pad
	XYPadPtr m_XYPad1;

	ScopedPointer<Label> m_XYpositionLabel;

	// Create container for our flex buttons
	ScopedPointer<EffectButtonContainerComponent> m_effectButtonContainer;

	// Define midi output device
	MidiOutput* m_midiOutput;

	// Define a fiter object
	ScopedPointer<IIRFilter> m_testFilter;

	double m_sampleRate;

	void playNote(int note);

	double scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)

};
