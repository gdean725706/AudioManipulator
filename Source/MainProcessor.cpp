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
	m_numberOfBuffers(3),
	m_writingToBuffer(false),
	m_samplesWritten(0),
	m_floatBuffer(44100 * 10),
	m_bufferIndex(0),
	m_savedBuffers(m_numberOfBuffers),
	m_effectChain1(this)
{

	//m_effectChains.push_back(EffectChain());
	// Initialise IIR Filter
	m_testFilter = new IIRFilter();

	m_speed.active = false;
	m_speed.type = FXType::Speed;
	m_LPF.active = false;
	m_LPF.type = FXType::LowPassFilter;
	m_HPF.active = false;
	m_HPF.type = FXType::HighPassFilter;

	m_simpleEffects.push_back(&m_speed);
	m_simpleEffects.push_back(&m_LPF);
	m_simpleEffects.push_back(&m_HPF);

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

	m_floatBuffer.reserve(sampleRate * 10);

	m_effectChain1.prepareToPlay(sampleRate, samplesPerBlock);

	for (int i = 0; i < m_numberOfBuffers; ++i)
	{
		m_savedBuffers[i].setSampleRate(sampleRate);
	}
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

	//set up phasor speed playback control
	if (m_speed.active)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (m_savedBuffers[i].isActive() == true)
			{
				m_savedBuffers[i].setSpeedMultiplier(scaleRange(m_padX, 0.0f, 1.0f, -2.0f, 4.0f));
			}
		}
	}

	IIRCoefficients ic;
	bool filterEnabled = false;
	// Get currently selected filter and set up
	if (m_LPF.active)
	{
		ic = IIRCoefficients::makeLowPass(m_sampleRate, freq, res);
		filterEnabled = true;
	}
	else if (m_HPF.active)
	{
		ic = IIRCoefficients::makeHighPass(m_sampleRate, freq, res);
		filterEnabled = true;
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

			if (m_writingToBuffer)
			{
				// Prevent overflow of 5 second buffer
				// TODO: way to link this to UI component to sync button state
				// and stoprecording() method
				if (m_samplesWritten <= m_floatBuffer.size())
				{
					m_floatBuffer.push_back(inBuffer[sample]);
					m_samplesWritten++;
				}
				else
				{
					m_writingToBuffer = false;
				}
			}
			
			for (int i = 0; i < 3; ++i)
			{
				if (m_savedBuffers[i].isActive() == true)
				{
					outBuffer[sample] += m_savedBuffers[i].getNextSampleCubic();
				}
			}

		}

		if (filterEnabled)
			m_testFilter->processSamples(outBuffer, buffer.getNumSamples());

	}

	m_effectChain1.process(buffer, midiMessages);

	/*if (currentEffect == FXType::Delay)
	{
		m_effectChain1.processDelay(buffer, midiMessages);
	}
	else if (currentEffect == FXType::Flanger)
	{
		m_effectChain1.processFlanger(buffer, midiMessages);
	}
	else if (currentEffect == FXType::PitchShift)
	{
		m_effectChain1.processPitchShifter(buffer, midiMessages);
	}
	else if (currentEffect == FXType::RingMod)
	{
		m_effectChain1.processRingMod(buffer, midiMessages);
	}*/

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

	XmlElement xmlMain("AM1_SETTINGS");

	xmlMain.setAttribute("PadX", m_padX);
	xmlMain.setAttribute("PadY", m_padY);

	// Track node position
	int index = 0;

	// Button States
	if (m_editor != nullptr)
	{
		auto* content = dynamic_cast<MainContentComponent*>(m_editor);

		for (auto* button : content->getEffectButtonContainer()->getButtons())
		{
			// Use button name as tag name, replacing spaces with underscores
			String buttonName = (String)button->get()->getName().replaceCharacter(' ', '_');
			xmlMain.addChildElement( new XmlElement( buttonName ) );
			xmlMain.getChildElement(index)->setAttribute("Active",(int)button->get()->getActive());

			++index;
		}
	}

	// DSP Effect States
	for (auto* effect : m_effectChain1.getAllEffects())
	{
		xmlMain.addChildElement(new XmlElement((String)"Effect_" + (String)effect->getType()));
		xmlMain.getChildElement(index)->setAttribute("Enabled", effect->isActive());

		for (auto* param : effect->getAllParameters())
		{
			xmlMain.getChildElement(index)->setAttribute((String)param->name.replaceCharacter(' ', '_'), *param);
		}
		++index;
	}

	copyXmlToBinary(xmlMain, destData);
}

void MainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	int index = 0;
	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("AM1_SETTINGS"))
		{
			setXY(xmlState->getDoubleAttribute("PadX", m_padX), xmlState->getDoubleAttribute("PadY", m_padY));
			if (m_editor != nullptr)
			{
				auto* content = dynamic_cast<MainContentComponent*>(m_editor);
				for (auto* button : content->getEffectButtonContainer()->getButtons())
				{
					String buttonName = (String)button->get()->getName().replaceCharacter(' ', '_');
					bool state = xmlState->getChildByName(buttonName)->getBoolAttribute("Active");
					button->get()->setActive(state);

					++index;
				}
			}

			for (auto* effect : m_effectChain1.getAllEffects())
			{
				if (xmlState->getChildElement(index)->hasTagName((String)"Effect_" + (String)effect->getType()))
				{
					for (auto* param : effect->getAllParameters())
					{
						*param = xmlState->getChildElement(index)->getDoubleAttribute((String)param->name.replaceCharacter(' ', '_'));
					}
				}
				++index;
			}
		}
	}

	DBG("Loaded state successfully");
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

EffectChain* MainAudioProcessor::getChain(int num)
{
	return &m_effectChain1;

	// Not yet implemented
	if (num < m_numberOfChains)
		return &m_effectChains[num];
}

void MainAudioProcessor::startRecording(int index)
{
	if (!m_writingToBuffer)
	{
		index = index > 3 ? 3 : index < 0 ? 0 : index;
		m_samplesWritten = 0;
		// Clean write cache buffer ready for new write
		// Don't clear save buffer until audio recoridng is complete
		m_floatBuffer.clear();
		m_writingToBuffer = true;
	}
}

void MainAudioProcessor::startPlayback(int index)
{
	m_savedBuffers[index].setActive(true);
}

void MainAudioProcessor::stopPlayback(int index)
{
	m_savedBuffers[index].setActive(false);
}

void MainAudioProcessor::stopRecording(int index)
{
	index = index > 3 ? 3 : index < 0 ? 0 : index;
	m_savedBuffers[index].fillBuffer(m_floatBuffer, m_samplesWritten);

	// Clear and copy cache buffer into store buffer
	//DBG(m_samplesWritten);
	// seconds of audio -  num channels
	//DBG((m_samplesWritten / m_sampleRate) / 2);
	// Clear and reset cache

	m_samplesWritten = 0;
	m_floatBuffer.clear();
	m_writingToBuffer = false;
}

void MainAudioProcessor::registerParameter(AudioProcessorParameter* parameter)
{
	addParameter(parameter);
}

void MainAudioProcessor::setSimpleEffectState(EffectBase::EffectType effect, bool active)
{
	for (int i = 0; i < m_simpleEffects.size(); ++i)
	{
		if (m_simpleEffects[i]->type == effect)
			m_simpleEffects[i]->active = active;
	}
}

void MainAudioProcessor::setMainComponent(AudioProcessorEditor* editor)
{
	m_editor = editor;
}

AudioProcessorEditor* MainAudioProcessor::getMainComponent()
{
	return m_editor;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MainAudioProcessor();
}
