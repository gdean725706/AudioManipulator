/*
  ==============================================================================

    MainProcessor.h
    Created: 8 Nov 2017 5:55:31pm
    Author:  deano

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectChain.h"
#include <vector>

//==============================================================================
/*
*/
class MainAudioProcessor    : public AudioProcessor
{
public:
    MainAudioProcessor();
    ~MainAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void setCurrentEffect(EffectBase::EffectType effect)
	{
		m_currentEffect = effect;
	}

	void setXYValues(float x, float y)
	{
		m_padX = x;
		m_padY = y;
	}

	void setMidiOutput(MidiOutput* midiOut)
	{
		m_midiOutput = midiOut;
	}

	void getChain(EffectChain* chain, int num)
	{
		if (num < m_numberOfChains)
			chain = &m_effectChains[num];
	}

private:

	// Typedef for our enum containing all the FX types
	typedef EffectBase::EffectType FXType;

	// Define midi output device
	MidiOutput* m_midiOutput;

	// Define a fiter object
	ScopedPointer<IIRFilter> m_testFilter;
	double m_filterFreq, m_filterRes;

	double m_sampleRate;

	void playNote(int note);

	double scaleRange(double input, double inputStart, double inputEnd, double outputStart, double outputEnd);

	float m_padX, m_padY;
	FXType m_currentEffect;

	int m_numberOfChains;
	std::vector<EffectChain> m_effectChains;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAudioProcessor)
};
