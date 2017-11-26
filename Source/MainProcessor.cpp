/*
  ==============================================================================

    MainProcessor.cpp
    Created: 8 Nov 2017 5:55:31pm
    Author:  George Dean

  ==============================================================================
*/

#include "MainProcessor.h"
#include "MainComponent.h"

//==============================================================================
MainAudioProcessor::MainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
			#if ! JucePlugin_IsMidiEffect
			#if ! JucePlugin_IsSynth
					.withInput("Input", AudioChannelSet::stereo(), true)
			#endif
					.withOutput("Output", AudioChannelSet::stereo(), true)
			#endif
				)
#endif
	,
	m_padX(0),
	m_padY(0),
	m_currentEffect(FXType::LowPassFilter),
	m_numberOfChains(1),
	m_effectChains(m_numberOfChains)

{

	// Initialise IIR Filter
	m_testFilter = new IIRFilter();

	////Specify number of audio i/o channels
	//setAudioChannels(2, 2);
}

MainAudioProcessor::~MainAudioProcessor()
{
}

//==============================================================================
const String MainAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MainAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MainAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MainAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MainAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MainAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int MainAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MainAudioProcessor::setCurrentProgram(int index)
{
}

const String MainAudioProcessor::getProgramName(int index)
{
	return {};
}

void MainAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void MainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	m_sampleRate = sampleRate;

	m_effectChains[0].prepareToPlay(sampleRate, samplesPerBlock);

}

void MainAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MainAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	const int maxInputChannels = getTotalNumInputChannels();
	const int maxOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.

	for (int i = maxInputChannels; i < maxOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// Set up the filter
	const double freq = scaleRange(m_padX, 0.0f, 1.0f, 20.0f, 20000.0f) + 0.0001f;
	const double res = scaleRange(m_padY, 0.0f, 1.0f, 0.5f, 2.0f) + 0.0001f;

	IIRCoefficients ic;
	bool filterEnabled = false;
	// Get currently selected filter and set up
	FXType currentEffect =  m_currentEffect;
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
		// Get pointers to our in/out buffer locations
		const float* inBuffer = buffer.getReadPointer(channel, 0);
		float* outBuffer = buffer.getWritePointer(channel, 0);

		//Output our input samples * volume scale & apply filtering
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			outBuffer[sample] = inBuffer[sample] * 0.9f;
		}

		if (filterEnabled)
			m_testFilter->processSamples(outBuffer, buffer.getNumSamples());

	}

	if (currentEffect == FXType::Delay)
	{
		m_effectChains[0].processDelay(buffer, midiMessages);
	}

}

//==============================================================================
bool MainAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MainAudioProcessor::createEditor()
{
	return new MainContentComponent(*this);
}

//==============================================================================
void MainAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

void MainAudioProcessor::playNote(int note)
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

double MainAudioProcessor::scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
	return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
}

void MainAudioProcessor::setCurrentEffect(EffectBase::EffectType effect)
{
	m_currentEffect = effect;
}

void MainAudioProcessor::setXY(float x, float y)
{
	m_padX = x;
	m_padY = y;
}

void MainAudioProcessor::setMidiOutput(MidiOutput* midiOut)
{
	m_midiOutput = midiOut;
}

void MainAudioProcessor::getChain(EffectChain* chain, int num)
{
	if (num < m_numberOfChains)
		chain = &m_effectChains[num];
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MainAudioProcessor();
}
