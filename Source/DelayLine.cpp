/*
  ==============================================================================

    DelayLine.cpp
    Created: 6 Nov 2017 6:08:07pm
    Author:  George Dean

  ==============================================================================
*/

#include "DelayLine.h"

StereoDelay::StereoDelay(int maxDelay, AudioProcessor* processor) :
	EffectBase(processor),
	m_leftDelay(maxDelay),
	m_rightDelay(maxDelay)
{
	// set up audio parameters
	addParameter(m_feedbackLevel = new AudioParameterFloat("Feedback Level", "Feedback Level", 0.0f, 0.9999f, 0.303f));
	addParameter(m_delayTime = new AudioParameterFloat("Delay Time", "Delay Time", 0.0f, 2000.0f, 1.0f));
	registerParameter(m_feedbackLevel);
	registerParameter(m_delayTime);
}

StereoDelay::~StereoDelay()
{

}


void StereoDelay::prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock)
{
	m_sampleRate = sampleRate;
	m_leftDelay.clearBuffer();
	m_rightDelay.clearBuffer();
}

void StereoDelay::releaseResources()
{
	m_leftDelay.clearBuffer();
	m_rightDelay.clearBuffer();
}

void StereoDelay::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	float* leftChannel = buffer.getWritePointer(0, 0);
	float* rightChannel = buffer.getWritePointer(1, 0);

	const float dt = *m_delayTime;
	const float fl = *m_feedbackLevel;

	for (int i = 0; i < buffer.getNumSamples(); ++i)
	{
		leftChannel[i] += m_leftDelay.getDelay(dt * (m_sampleRate * 0.001f));
		rightChannel[i] += m_rightDelay.getDelay(dt * (m_sampleRate * 0.001f));
		m_leftDelay.writeSample(leftChannel[i] * fl);
		m_rightDelay.writeSample(rightChannel[i] * fl);
		m_leftDelay.tick();
		m_rightDelay.tick();
	}
}


double StereoDelay::getTailLengthSeconds() const
{
	return 0.0;
}

