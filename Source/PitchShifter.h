/*
  ==============================================================================

    PitchShifter.h
    Created: 12 Feb 2018 3:45:01pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once
//fout = fin * (1 - pfreq * (dw)/1000)
#include "JuceHeader.h"
#include "Phasor.h"
#include "RecordingBuffer.h"
#include "DelayUnit.h"
#include "EffectBase.h"

class PitchShifter : public EffectBase
{
public:
	PitchShifter(AudioProcessor* processor) :
		EffectBase(processor),
		m_transpose(44100.0f/2048.0f),
		m_delayTime(2048),
		m_leftDelay(2048),
		m_rightDelay(2048),
		m_window(512),
		m_phasor(44100.0f, 1.0f)
	{
		// Hannng to ensure sum of overlapping windows is constant
		m_window.fillHann();
	}

	~PitchShifter()
	{

	}

	void prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock)
	{
		m_transpose = sampleRate / 2048.0f;
	}

	void releaseResources()
	{
	}

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		float* leftChannel = buffer.getWritePointer(0, 0);
		float* rightChannel = buffer.getWritePointer(1, 0);

		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			m_leftDelay.writeSample(leftChannel[i]);
			m_rightDelay.writeSample(rightChannel[i]);

			float phase = m_phasor.getPhase();
			float offsetPhase = wrap(phase + 0.5);

			float leftDelay = m_leftDelay.getDelay(phase * m_delayTime);
			float rightDelay = m_rightDelay.getDelay(phase * m_delayTime);
			
			float invleftDelay = m_leftDelay.getDelay(offsetPhase * m_delayTime);
			float invrightDelay = m_rightDelay.getDelay(offsetPhase * m_delayTime);

			float window = m_window.getSample(phase * m_window.getSize());
			float offsetWindow = m_window.getSample(offsetPhase * m_window.getSize());

			leftChannel[i] += (leftDelay * window + invleftDelay * offsetWindow);
			rightChannel[i] += (rightDelay * window + invrightDelay * offsetWindow);

			m_phasor.tick();
			m_leftDelay.tick();
			m_rightDelay.tick();
		}
	}

	//http://msp.ucsd.edu/techniques/v0.11/book-html/node125.html
	//http://www.katjaas.nl/pitchshift/pitchshift.html
	void setFrequency(float frequency)
	{
		m_phasor.setFrequency((((pow(2.0f, frequency / 12.0f) - 1)*-1.0f)*m_transpose));
	}

	double getTailLengthSeconds() const
	{
		return 0.0;
	}

	EffectType getType() override
	{
		return EffectType::PitchShifter;
	}

private:
	int m_transpose, m_delayTime;
	DelayUnit m_leftDelay, m_rightDelay;
	RecordingBuffer m_window;
	Phasor m_phasor;

};