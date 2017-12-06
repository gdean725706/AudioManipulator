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
	m_rightDelay(maxDelay),
	m_fbk(0),
	m_dt(0)
{
	// set up audio parameters
	addParameter(m_feedbackLevel = new AudioParameterFloat("Feedback Level", "Feedback Level", 0, 0.9999, 0.303));
	addParameter(m_delayTime = new AudioParameterFloat("Delay Time", "Delay Time", 0, 2000, 1));
	
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

	const float dt = m_dt;
	const float fl = m_fbk;

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

void StereoDelay::updateValues(float x, float y)
{
	String xstr, ystr;
	xstr << *m_feedbackLevel;
	ystr << *m_delayTime;
	Logger::writeToLog("feedback was " + xstr + " . dt was " + ystr);

	*m_feedbackLevel = x;
	*m_delayTime = y;
	m_fbk = x;
	m_dt = y;

	xstr = "";
	ystr = "";

	xstr << *m_feedbackLevel;
	ystr << *m_delayTime;
	Logger::writeToLog("feedback set to " + xstr + " . dt set to " + ystr);
}


double StereoDelay::getTailLengthSeconds() const
{
	return 0.0;
}

