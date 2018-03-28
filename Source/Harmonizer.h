/*
  ==============================================================================

    Harmonizer.h
    Created: 26 Mar 2018 6:42:23pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "PitchShifter.h"

class Harmonizer : public EffectBase
{
public:
	Harmonizer(AudioProcessor* processor) :
		EffectBase(processor),
		m_harmony1(processor),
		m_harmony2(processor)
	{
		addParameter(m_harmony1Pitch = new AudioParameterFloat("Harmonizer1Pitch", "Harmonizer 1 Pitch", -16, 16, 0));
		addParameter(m_harmony2Pitch = new AudioParameterFloat("Harmonizer2Pitch", "Harmonizer 2 Pitch", -16, 16, 0));
		
		registerParameter(m_harmony1Pitch);
		registerParameter(m_harmony2Pitch);

		m_harmony1.setDryMix(1.0f);
		m_harmony2.setDryMix(0.0f);
	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override
	{
		m_harmony1.prepareToPlay(sampleRate, maxExpectedSamplesPerBlock);
		m_harmony2.prepareToPlay(sampleRate, maxExpectedSamplesPerBlock);
	}

	void releaseResources() override
	{
		m_harmony1.releaseResources();
		m_harmony2.releaseResources();
	}

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{
		float* leftChannel = buffer.getWritePointer(0, 0);
		float* rightChannel = buffer.getWritePointer(1, 0);

		m_harmony1.processBlock(buffer, midiMessages);
		m_harmony2.processBlock(buffer, midiMessages);

		// Reduce gain 
		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			leftChannel[i] *= 0.33f;
			rightChannel[i] *= 0.33f;
		}
	}

	void updateFrequencies()
	{
		m_harmony1.setFrequency(*m_harmony1Pitch);
		m_harmony2.setFrequency(*m_harmony2Pitch);
	}

	double getTailLengthSeconds() const
	{
		return 0.0;
	}

	EffectType getType() override
	{
		return EffectType::Harmonizer;
	}
private:
	PitchShifter m_harmony1, m_harmony2;
	AudioParameterFloat* m_harmony1Pitch, *m_harmony2Pitch;
};
