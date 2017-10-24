/*
  ==============================================================================

	DelayUnit.h
	Created: 30 Oct 2015 1:10:19pm
	Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DelayUnit
{

	float* m_delayBuffer;
	int m_maxDelay;
	int m_writeLocation;


public:
	DelayUnit(int maxDelay) : m_maxDelay(maxDelay), m_writeLocation(0) {

		m_delayBuffer = new float[m_maxDelay];

		clearBuffer();

	}

	void clearBuffer() {
		for (int n = 0; n < m_maxDelay; n++) {
			m_delayBuffer[n] = 0;
		}
	}
	void writeSample(float sample) {
		m_delayBuffer[m_writeLocation] = sample;
	}

	// Makes a Circular Buffer
	void tick() {
		m_writeLocation += 1;

		while (m_writeLocation >= m_maxDelay) {
			m_writeLocation -= m_maxDelay;
		}
		while (m_writeLocation < 0) {
			m_writeLocation += m_maxDelay;
		}
	}

	float getDelay(int delayTime) {
		int pos = m_writeLocation - delayTime; // position in sound we're delaying from

		while (pos < 0) {
			pos += m_maxDelay; //Wraps round, stops pos entering neg numbers
		}
		while (pos > m_maxDelay) {
			pos -= m_maxDelay; // Keeps buffer in range - reading wraps round
		}
		return m_delayBuffer[pos];

	}

	~DelayUnit() {
		delete[] m_delayBuffer; //Array Cleanup
		m_delayBuffer = NULL;
	}
};