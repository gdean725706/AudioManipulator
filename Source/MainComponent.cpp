/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "MainProcessor.h"

//==============================================================================
MainContentComponent::MainContentComponent(MainAudioProcessor& p) 
	: AudioProcessorEditor(&p), processor(p)
{
	m_settingsButton = new TextButton("Settings");

	// Create controls container component
	m_controlsContainer = new ControlContainerComponent();

	// Create effect button container
	m_effectButtonContainer = new EffectButtonContainerComponent(600, 400);

	// set up pointer to XY pad in controls container
	m_XYPad1 = m_controlsContainer->getXYPad();
	m_XYPad1->setMainAudioProcessor(&p);

	// Make our components visible
	addAndMakeVisible(m_settingsButton);
	addAndMakeVisible(m_controlsContainer);
	addAndMakeVisible(m_effectButtonContainer);

	// flexWrap CSS equiv attribute
	flexWrap = Wrap::wrap;
	alignContent = AlignContent::spaceBetween;
	justifyContent = JustifyContent::center;

	// Set up our button click event observers
	m_settingsButton->addListener(this);

	setSize(1024, 768);

}

MainContentComponent::~MainContentComponent()
{

}


void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

	String valueFreq, valueRes;
	valueFreq << scaleRange(m_XYPad1->getXValueNormalised(), 0.0f, 1.0f, 0.0f, 18000.0f) + 0.0001f;
	valueRes << scaleRange(m_XYPad1->getYValueNormalised(), 0.0f, 1.0f, 0.0f, 24.0f) + 0.0001f;
	g.drawText(valueFreq + " , " + valueRes , getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	auto bounds = getLocalBounds();
	int headerHeight = getHeight() * 0.05f;
	auto header = bounds.removeFromTop(headerHeight);

	auto settingsButtonBounds = header.removeFromLeft(100).reduced(3);
	m_settingsButton->setBounds(settingsButtonBounds);

	//bounds.removeFromTop(m_XYPad1->getHeight()).reduced(3);
	m_effectButtonContainer->setBounds(bounds.removeFromTop(100).reduced(3));
	m_controlsContainer->setBounds(bounds.removeFromTop(400).reduced(3));

	int buttonHeight = 10;

	// Call flexbox perform layout method, as we have already set up the list of items
	// flexbox will magically arrange for us
	performLayout(bounds);

}

// Callback for button clicked
void MainContentComponent::buttonClicked(Button * button)
{
	if (button == m_settingsButton)
	{
		//// Create our settings selector object and define our input parameters
		//ScopedPointer<AudioDeviceSelectorComponent> settingsSelect = new AudioDeviceSelectorComponent(deviceManager,1,2,1,2,true,true,true,false);
		//// set arbitrary dialog window size
		//settingsSelect->setSize(600, 600);
		//// Create a window containing our settings select component
		//DialogWindow::showModalDialog("Audio Settings", settingsSelect, this, Colours::cadetblue, true, true, true);

		//// Get our midi output device from audio manager
		//processor.setMidiOutput(deviceManager.getDefaultMidiOutput());
	}

}

void MainContentComponent::mouseMove(const MouseEvent& evt)
{
	repaint();
}

double MainContentComponent::scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
	return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
}
