/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYPadComponent.h"

class TestComponent : public Button, public FlexItem
{
private:
	Colour m_colour;
	int m_midiPitch;
public:
	TestComponent(int pitch) :
		FlexItem(100, 25),
		Button("Midi Button"),
		m_midiPitch(pitch),
		m_colour(Colours::lightgrey)
	{
		// Randomize colour
		//Random r;
		//m_colour = Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));

		// Due to FlexBox multiple inheritance we must set the associated component to this
		associatedComponent = this;

		alignSelf = AlignSelf::autoAlign;
	}
	// Override paint virtual to make it do what we want it to do
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
	{
		// Set up button colour to change based on mouse events
		if (isButtonDown){
			m_colour = Colours::black;
		}
		else{
			m_colour = Colours::grey;
		}

		if (isMouseOverButton)
		{
			m_colour = m_colour.brighter();
		}

		// fill colours
		g.fillAll(m_colour);

		// dynamic casting stream int to string
		String text;
		text << m_midiPitch;

		g.setColour(Colours::hotpink);
		g.drawText(text, getLocalBounds(), Justification::centred, false);
	}

	int getPitch() {
		return m_midiPitch;
	}
};


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

	void buttonClicked(Button* button) override;

private:
	ScopedPointer<TextButton> m_settingsButton;
	// Define our own scopedpointer custom componenet
	typedef ScopedPointer<TestComponent> TestPtr;
	typedef ScopedPointer<XYPadComponent> XYPadPtr;

	// Our test objects
	//TestPtr m_custom1, m_custom2, m_custom3, m_custom4, m_custom5;

	// Create XY Pad
	XYPadPtr m_XYPad1;

	ScopedPointer<Label> m_XYpositionLabel;

	// Define our audio device manager - this deals with the audio i/o for us
	AudioDeviceManager m_audioManager;

	// Define midi output device
	MidiOutput* m_midiOutput;

	// Define a fiter object
	IIRFilter* m_testFilter;

	double m_sampleRate;

	void playNote(int note);

	double scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)

};
