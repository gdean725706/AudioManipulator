/*
  ==============================================================================

    RingModulator.h
    Created: 23 Mar 2018 12:32:20am
    Author:  George Dean

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "RecordingBuffer.h"
#include "EffectBase.h"


class RingModulator : public EffectBase
{
public:
	RingModulator(AudioProcessor* processor) :
		EffectBase(processor),
		m_sineOscillator(512),
		m_phasor(44100.0f, 1000),
		m_phase(0)

	{
		
		addParameter(m_modFrequency = new AudioParameterFloat("Mod Frequency", "Mod Frequency", 100, 5000, 1000));
		addParameter(m_modDepth = new AudioParameterFloat("Mod Depth", "Mod Depth", 0, 100, 50));
		registerParameter(m_modFrequency);
		registerParameter(m_modDepth);

		m_sineOscillator.fillSine();
	}

	~RingModulator()
	{

	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock) override
	{
		m_sineOscillator.setSampleRate(sampleRate);
		m_phasor.setSampleRate(sampleRate);
	}

	void releaseResources() override
	{

	}

	// wavetable isn't working properly pls fix in the morning
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{

		float* leftChannel = buffer.getWritePointer(0, 0);
		float* rightChannel = buffer.getWritePointer(1, 0);

		const float depth = *m_modDepth;
		const float speed = *m_modFrequency;

		m_phasor.setFrequency(speed);

		float osc = 0.0f;

		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{

			osc = m_sineOscillator.getSample(m_phasor.getPhase() * m_sineOscillator.getSize());
			leftChannel[i] *= (osc * (depth * 0.01f));
			rightChannel[i] *= (osc * (depth * 0.01f));
			m_phasor.tick();

			m_phase++;

		}

	}

	double getTailLengthSeconds() const
	{
		return 0.0;
	}

	EffectType getType() override
	{
		return EffectType::RingMod;
	}

private:
	AudioParameterFloat* m_modFrequency, *m_modDepth;
	Wavetable m_sineOscillator;
	Phasor m_phasor;
	float m_phase;
};