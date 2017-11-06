/*
  ==============================================================================

    DelayLine.h
    Created: 6 Nov 2017 6:08:07pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DelayLine : public AudioProcessor
{
private:
	ScopedPointer<float> m_delayBuffer;
	int m_maxDelay;
	int m_writeLocation;
	
public:
	DelayLine(int maxDelay);

	void clearBuffer();

	void writeSample(float sample);

	void tick();

	float getDelay(int delayTime);

};