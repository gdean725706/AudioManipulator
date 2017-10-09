/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	m_settingsButton = new TextButton("Settings");

	// Create test component objects
	m_custom1 = new TestComponent(48);
	m_custom2 = new TestComponent(50);
	m_custom3 = new TestComponent(51);
	m_custom4 = new TestComponent(53);
	m_custom5 = new TestComponent(55);

	// Make our components visible
	addAndMakeVisible(m_settingsButton);
	addAndMakeVisible(m_custom1);
	addAndMakeVisible(m_custom2);
	addAndMakeVisible(m_custom3);
	addAndMakeVisible(m_custom4);
	addAndMakeVisible(m_custom5);

	// Add them to our flexbox list (inherited)
	items.add(m_custom1->withMargin(3));
	items.add(m_custom2->withMargin(3));
	items.add(m_custom3->withMargin(3));
	items.add(m_custom4->withMargin(3));
	items.add(m_custom5->withMargin(3));

	// flexWrap CSS equiv attribute
	flexWrap = Wrap::wrap;
	alignContent = AlignContent::spaceBetween;

	// Set up our button click event observers
	m_settingsButton->addListener(this);
	m_custom1->addListener(this);
	m_custom2->addListener(this);
	m_custom3->addListener(this);
	m_custom4->addListener(this);
	m_custom5->addListener(this);

    setSize (600, 400);

	//Specify number of audio i/o channels
	setAudioChannels(2, 2);
}

MainContentComponent::~MainContentComponent()
{
	shutdownAudio();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Clear buffer to prevent random noise output.
	bufferToFill.clearActiveBufferRegion();
}

void MainContentComponent::releaseResources()
{

}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	auto bounds = getLocalBounds();
	int headerHeight = getHeight() * 0.2f;
	auto header = bounds.removeFromTop(headerHeight);

	m_settingsButton->setBounds(header.removeFromLeft(100).reduced(3));

	int buttonHeight = 30;

	// Call flexbox perform layout method, as we have already set up the list of items
	// flexbox will magically arrange for us
	performLayout(bounds);

}

// Callback for button clicked
void MainContentComponent::buttonClicked(Button * button)
{
	if (button == m_settingsButton)
	{
		// Create our settings selector object and define our input parameters
		ScopedPointer<AudioDeviceSelectorComponent> settingsSelect = new AudioDeviceSelectorComponent(m_audioManager,1,2,1,2,true,true,true,false);
		// set arbitrary dialog window size
		settingsSelect->setSize(600, 600);
		// Create a window containing our settings select component
		DialogWindow::showModalDialog("Audio Settings", settingsSelect, this, Colours::cadetblue, true, true, true);

		// Get our midi output device from audio manager
		m_midiOutput = m_audioManager.getDefaultMidiOutput();
	}
	else if (button == m_custom1) {
		playNote(m_custom1->getPitch());
	}
	else if (button == m_custom2) {
		playNote(m_custom2->getPitch());
	}
	else if (button == m_custom3) {
		playNote(m_custom3->getPitch());
	}
	else if (button == m_custom4) {
		playNote(m_custom4->getPitch());
	}
	else if (button == m_custom5) {
		playNote(m_custom5->getPitch());
	}

}

void MainContentComponent::playNote(int note)
{
	if (m_midiOutput)
	{
		MidiMessage msg = MidiMessage::noteOn(1, note, (uint8)100);
		m_midiOutput->sendMessageNow(msg);
		
		Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100);

		MidiMessage msgOff = MidiMessage::noteOff(1, note);
		m_midiOutput->sendMessageNow(msgOff);
	}
}
