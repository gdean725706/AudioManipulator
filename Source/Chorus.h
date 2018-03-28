/*
  ==============================================================================

    Chorus.h
    Created: 20 Jan 2018 88:-88:37am
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "EffectBase.h"
#include "DelayUnit.h"
#include "RecordingBuffer.h"

class Chorus : public EffectBase
{
	AudioParameterFloat* m_feedback, *m_depth, *m_frequency;
	DelayUnit m_leftDelay, m_rightDelay;
	Wavetable m_lfo;
	Phasor m_phasor;
public:
	Chorus(AudioProcessor* processor) :
		EffectBase(processor),
		m_leftDelay(2048),
		m_rightDelay(2048),
		m_lfo(512),
		m_phasor()
	{
		addParameter(m_feedback = new AudioParameterFloat("Chorus Level", "Chorus Level", 0.0f, 1.0f, 0.5f));
		registerParameter(m_feedback);
		addParameter(m_depth = new AudioParameterFloat("Chorus Depth", "Chorus Depth", 0.0f, 1.0f, 0.5f));
		registerParameter(m_depth);
		addParameter(m_frequency = new AudioParameterFloat("Chorus Frequency", "Chorus Frequency", 0.02f, 5.0f, 0.18f));
		registerParameter(m_frequency);

		m_lfo.fillSine();
	}
	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override
	{
		m_phasor.setSampleRate(sampleRate);
	}

	void releaseResources() override
	{
		m_leftDelay.clearBuffer();
		m_rightDelay.clearBuffer();

	}

	// wavetable isn't working properly pls fix in the morning
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{

		float* leftChannel = buffer.getWritePointer(0, 0);
		float* rightChannel = buffer.getWritePointer(1, 0);

		m_phasor.setFrequency(*m_frequency);

		float leftSample(0), rightSample(0), inverseFeedback(0);

		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			inverseFeedback = 1.0f - *m_feedback;
			float delayPos = m_lfo.getSample(m_phasor.getPhase() * m_lfo.getSize()) * *m_depth + *m_depth + 1.0f;
			
			leftSample = leftChannel[i];
			rightSample = rightChannel[i];

			leftChannel[i] *= inverseFeedback + m_leftDelay.getDelay(delayPos) * *m_feedback;
			rightChannel[i] *= inverseFeedback + m_rightDelay.getDelay(delayPos) * *m_feedback;

			m_leftDelay.writeSample(leftSample);
			m_rightDelay.writeSample(rightSample);

			m_phasor.tick();
			m_leftDelay.tick();
			m_rightDelay.tick();
		
		}

	}

	double getTailLengthSeconds() const
	{
		return 0.0;
	}

	EffectType getType() override
	{
		return EffectType::Chorus;
	}
};