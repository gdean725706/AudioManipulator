/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent(MainAudioProcessor& p) 
	: AudioProcessorEditor(&p), processor(p)
{
	processor.setMainComponent(this);

	// Create controls container component
	m_controlsContainer = new ControlContainerComponent(&p, 0);

	// Create effect button container
	m_effectButtonContainer = new EffectButtonContainerComponent(&p, 0);

	m_bottomContainer = new BottomContainer(&p);

	// Make our components visible
	addAndMakeVisible(m_controlsContainer);
	addAndMakeVisible(m_effectButtonContainer);
	addAndMakeVisible(m_bottomContainer);

	// flexWrap CSS equiv attribute
	flexWrap = Wrap::wrap;
	alignContent = AlignContent::spaceBetween;
	justifyContent = JustifyContent::center;

	// Set window sizing constraints
	setResizeLimits(661, 496, 1180, 700);
	setSize(872, 546);

	// Attach to OpenGL renderer to improve graphics performance
	m_glContext.attachTo(*this);

#if JUCE_ANDROID
    Desktop::getInstance().setScreenSaverEnabled(false);
	//Desktop::getInstance().setOrientationsEnabled(4 | 8 );
	//Desktop::getInstance().setKioskModeComponent(this, false);
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

	// Get this windows' bounds and set the containers height as a %age
	auto bounds = getLocalBounds();
	int effectContainerHeight = getHeight() * 0.15f;
	int controlContainerHeight = getHeight() * 0.7f;
	int bottomContainerHeight = getHeight() * 0.15f;

	//Effect button and controls at the top of the window
	m_effectButtonContainer->setBounds(bounds.removeFromTop(effectContainerHeight).reduced(2));
	m_controlsContainer->setBounds(bounds.removeFromTop(controlContainerHeight).reduced(2));
	// Bottom container removes from bottom
	m_bottomContainer->setBounds(bounds.removeFromBottom(bottomContainerHeight).reduced(2));

	// Call flexbox perform layout method, as we have already set up the list of items
	// flexbox will magically arrange for us
	performLayout(bounds);

}

void MainContentComponent::mouseMove(const MouseEvent& evt)
{
	repaint();

}

// Callback for button clicked
void MainContentComponent::buttonClicked(Button * button)
{


}


EffectButtonContainerComponent* MainContentComponent::getEffectButtonContainer()
{
	return m_effectButtonContainer;
}

BottomContainer* MainContentComponent::getBottomContainer()
{
	return m_bottomContainer;
}

ControlContainerComponent* MainContentComponent::getControlContainer()
{
	return m_controlsContainer;
}