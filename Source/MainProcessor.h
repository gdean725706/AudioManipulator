/*
  ==============================================================================

    MainProcessor.h
    Created: 8 Nov 2017 5:55:31pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectChain.h"
#include <vector>
#include <stack>
#include "RecordingBuffer.h"


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

	void setCurrentEffect(EffectBase::EffectType effect);

	void setXY(float x, float y);

	void setMidiOutput(MidiOutput* midiOut);

	EffectChain* getChain(int num);

	void startRecording(int index);
	void stopRecording(int index);
	void startPlayback(int index);
	void stopPlayback(int index);

	void registerParameter(AudioProcessorParameter* parameter);

	struct SimpleEffect
	{
		bool active;
		EffectBase::EffectType type;
	};

	void setSimpleEffectState(EffectBase::EffectType effect, bool active);

	bool updated();

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

	float m_padX, m_padY;
	FXType m_currentEffect;

	int m_numberOfChains; // not yet implemented
	std::vector<EffectChain> m_effectChains;
	EffectChain m_effectChain1;

	int m_numberOfBuffers;
	bool m_writingToBuffer;
	int m_samplesWritten;
	std::vector<float> m_floatBuffer;
	int m_bufferIndex;

	std::vector<Wavetable> m_savedBuffers;

	SimpleEffect m_speed, m_LPF, m_HPF;
	std::vector<SimpleEffect*> m_simpleEffects;

	bool m_updateGUI;
	float m_lastX, m_lastY;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAudioProcessor)
};