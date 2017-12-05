/*
  ==============================================================================

    DelayLine.cpp
    Created: 6 Nov 2017 6:08:07pm
    Author:  George Dean

  ==============================================================================
*/

#include "DelayLine.h"

StereoDelay::StereoDelay(int maxDelay) :
	m_leftDelay(maxDelay),
	m_rightDelay(maxDelay)
{
	// set up audio parameters
	m_feedbackLevel = new AudioParameterFloat("Feedback Level", "Feedback Level", 0, 0.9999, 0.303);
	addParameter(m_feedbackLevel);
	m_delayTime = new AudioParameterFloat("Delay Time", "Delay Time", 0, 2000, 1);
	addParameter(m_delayTime);

	registerAudioParameters(getParameters());
	
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
	float* leftChannel = buffer.getWritePointer(0);
	float* rightChannel = buffer.getWritePointer(1);

	for (int i = 0; i < buffer.getNumSamples(); ++i)
	{
		leftChannel[i] += m_leftDelay.getDelay(m_delayTime->get() * (m_sampleRate * 0.001f));
		rightChannel[i] += m_rightDelay.getDelay(m_delayTime->get() * (m_sampleRate * 0.001f));
		m_leftDelay.writeSample(leftChannel[i] * m_feedbackLevel->get());
		m_rightDelay.writeSample(rightChannel[i] * m_feedbackLevel->get());

		m_leftDelay.tick();
		m_rightDelay.tick();
	}

}

double StereoDelay::getTailLengthSeconds() const
{
	return 0.0;
}