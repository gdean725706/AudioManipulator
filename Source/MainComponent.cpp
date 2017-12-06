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

	// Create controls container component
	m_controlsContainer = new ControlContainerComponent(p, 0);

	// Create effect button container
	m_effectButtonContainer = new EffectButtonContainerComponent(p, 600, 400);

	// Make our components visible
	addAndMakeVisible(m_controlsContainer);
	addAndMakeVisible(m_effectButtonContainer);

	// flexWrap CSS equiv attribute
	flexWrap = Wrap::wrap;
	alignContent = AlignContent::spaceBetween;
	justifyContent = JustifyContent::center;

	// Set up our button click event observers

	setSize(1024, 768);

	// Attach to OpenGL renderer to improve graphics performance
	m_glContext.attachTo(*this);

#if JUCE_ANDROID
    Desktop::getInstance().setScreenSaverEnabled(false);
#endif

}


MainContentComponent::~MainContentComponent()
{
    m_glContext.detach();
}


void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

	String valueFreq, valueRes;
	g.drawText(valueFreq + " , " + valueRes , getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	auto bounds = getLocalBounds();
	int effectContainerHeight = getHeight() * 0.08f;

	m_effectButtonContainer->setBounds(bounds.removeFromTop(effectContainerHeight).reduced(3));
	m_controlsContainer->setBounds(bounds.removeFromTop(400).reduced(3));
	int buttonHeight = 10;

	// Call flexbox perform layout method, as we have already set up the list of items
	// flexbox will magically arrange for us
	performLayout(bounds);

}

// Callback for button clicked
void MainContentComponent::buttonClicked(Button * button)
{


}

void MainContentComponent::mouseMove(const MouseEvent& evt)
{
	repaint();
}

double MainContentComponent::scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
	return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
}
