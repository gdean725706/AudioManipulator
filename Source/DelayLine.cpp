/*
  ==============================================================================

    DelayLine.cpp
    Created: 6 Nov 2017 6:08:07pm
    Author:  deano

  ==============================================================================
*/

#include "DelayLine.h"

DelayLine::DelayLine(int maxDelay) :
	m_maxDelay(maxDelay),
	m_writeLocation(0)
{
	m_delayBuffer = new float[m_maxDelay];
	clearBuffer();
}

void DelayLine::clearBuffer()
{
	for (int i = 0; i < m_maxDelay; ++i)
	{
		m_delayBuffer[i] = 0;
	}
}

void DelayLine::writeSample(float sample)
{
	m_delayBuffer[m_writeLocation] = sample;
}


void DelayLine::tick()
{
	// Makes a circular buffer

	m_writeLocation += 1;

	while (m_writeLocation >= m_maxDelay)
	{
		m_writeLocation -= m_maxDelay;
	}

	while (m_writeLocation < 0)
	{
		m_writeLocation += m_maxDelay;
	}
}

float DelayLine::getDelay(int delayTime)
{
	// Position in sound we're delaying from
	int pos = m_writeLocation - delayTime;

	while (pos < 0)
	{
		// Wraps around, stops pos entering neg numbers
		pos += m_maxDelay;
	}

	while (pos > m_maxDelay)
	{
		// Keeps buffer in range - reading wraps around
		pos -= m_maxDelay;
	}

	return m_delayBuffer[pos];
}