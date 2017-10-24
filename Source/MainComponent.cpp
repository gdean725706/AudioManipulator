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

	// Create XY Pad component
	m_XYPad1 = new XYPadComponent(400,400);

	m_XYpositionLabel = new Label("XYPosLbl", "");

	m_effectButtonContainer = new EffectButtonContainerComponent(600, 400);

	// Initialise IIR Filter
	m_testFilter = new IIRFilter();

	// Make our components visible
	addAndMakeVisible(m_settingsButton);
	addAndMakeVisible(m_XYPad1);
	addAndMakeVisible(m_XYpositionLabel);
	addAndMakeVisible(m_effectButtonContainer);

	// Add them to our flexbox list (inherited)
	items.add(m_XYPad1->withMargin(3));

	// flexWrap CSS equiv attribute
	flexWrap = Wrap::wrap;
	alignContent = AlignContent::spaceBetween;
	justifyContent = JustifyContent::center;

	// Set up our button click event observers
	m_settingsButton->addListener(this);

    setSize (1024, 768);

	//Specify number of audio i/o channels
	setAudioChannels(2, 2);
}

MainContentComponent::~MainContentComponent()
{
	shutdownAudio();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	m_sampleRate = sampleRate;
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Get current audio device
	AudioIODevice* device = deviceManager.getCurrentAudioDevice();
	// Juce uses bitmasking for active i/o channels
	const BigInteger activeInputChannels = device->getActiveInputChannels();
	const BigInteger activeOutputChannels = device->getActiveOutputChannels();
	// Inspect the bits, max channels will be highest bit +1
	const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
	const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

	// Set up the filter
	const double freq = scaleRange(m_XYPad1->getXValueNormalised(), 0.0f, 1.0f, 0.0f, 20000.0f) + 0.0001f;
	const double res = scaleRange(m_XYPad1->getYValueNormalised(), 0.0f, 1.0f, 1.0f, 8.0f) + 0.0001f;

	IIRCoefficients ic;
	bool filterEnabled = false;
	// Get currently selected filter and set up
	FXType currentEffect = m_effectButtonContainer->getCurrentEffect();
	switch (currentEffect)
	{
	case FXType::LowPassFilter:
		ic = IIRCoefficients::makeLowPass(m_sampleRate, freq, res);
		filterEnabled = true;
		break;
	case FXType::HighPassFilter:
		ic = IIRCoefficients::makeHighPass(m_sampleRate, freq, res);
		filterEnabled = true;
		break;
	default:
		filterEnabled = false;
	}
	
	if (filterEnabled)
		m_testFilter->setCoefficients(ic);

	// Cycle through each channel
	for (int channel = 0; channel < maxOutputChannels; ++channel)
	{
		// Check that we have an active channel to output to, if not - clear it
		if ((!activeOutputChannels[channel] || maxInputChannels == 0))
		{
			bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);	
		}
		else
		{
			// In case we are going to cycle through more output channels than there are input channels
			// Wrap current channel using Mod of max input channels
			const int actualInputChannel = channel & maxInputChannels;

			// Again clear the buffer if we have ourselves an inactive input channel
			if (!activeInputChannels[channel])
			{
				bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
			}
			// Now we can actually DO the processing
			else
			{
				// Get pointers to our in/out buffer locations
				const float* inBuffer = bufferToFill.buffer->getReadPointer(actualInputChannel, bufferToFill.startSample);
				float* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

				//Output our input samples * volume scale & apply filtering
				for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				{
					if (filterEnabled)
						outBuffer[sample] = m_testFilter->processSingleSampleRaw(inBuffer[sample]) * 0.9f;
					else
						outBuffer[sample] = inBuffer[sample] * 0.9f;
				}
			}
		}
	}

	// Clear buffer to prevent random noise output.
	//bufferToFill.clearActiveBufferRegion();
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
		// Create our settings selector object and define our input parameters
		ScopedPointer<AudioDeviceSelectorComponent> settingsSelect = new AudioDeviceSelectorComponent(deviceManager,1,2,1,2,true,true,true,false);
		// set arbitrary dialog window size
		settingsSelect->setSize(600, 600);
		// Create a window containing our settings select component
		DialogWindow::showModalDialog("Audio Settings", settingsSelect, this, Colours::cadetblue, true, true, true);

		// Get our midi output device from audio manager
		m_midiOutput = deviceManager.getDefaultMidiOutput();
	}

}

void MainContentComponent::mouseMove(const MouseEvent& evt)
{
	repaint();
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

double MainContentComponent::scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
	return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
}
