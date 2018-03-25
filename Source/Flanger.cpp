/*
  ==============================================================================

    Flanger.cpp
    Created: 7 Dec 2017 12:21:37am
    Author:  George Dean

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Flanger.h"

//==============================================================================
Flanger::Flanger(int maxDelay, AudioProcessor* processor) :
	EffectBase(processor),
	m_leftDelay(maxDelay),
	m_rightDelay(maxDelay),
	m_phase(0)
{
    // Set up audio parameters
	addParameter(m_feedback = new AudioParameterFloat("Feedback", "Feedback", 0.0f, 0.99f, 0.0f));
	registerParameter(m_feedback);
	addParameter(m_depth = new AudioParameterFloat("Depth", "Depth", 0.0f, 1.0f, 0.5f));
	registerParameter(m_depth);
	addParameter(m_frequency = new AudioParameterFloat("Frequency", "Frequency", 0.02f, 5.0f, 0.18f));
	registerParameter(m_frequency);

}

Flanger::~Flanger()
{

}


void Flanger::prepareToPlay(double sampleRate, int maxExpectedSamplesPerBlock)
{
	m_sampleRate = sampleRate;
	m_leftDelay.clearBuffer();
	m_rightDelay.clearBuffer();
}

void Flanger::releaseResources()
{
	m_leftDelay.clearBuffer();
	m_rightDelay.clearBuffer();
}

void Flanger::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	float* leftChannel = buffer.getWritePointer(0);
	float* rightChannel = buffer.getWritePointer(1);

	int delayPos(44100);
	float leftSample(0), rightSample(0), lfo(0);


	for (int i = 0; i < buffer.getNumSamples(); ++i)
	{
		lfo = sin(((2.0f * double_Pi) * m_phase * *m_frequency) / 44100); // LFO - make this into wavetable later

		delayPos = (lfo * *m_depth) + 440; //depth (offset by 440)

		leftSample = leftChannel[i] * 0.8 + m_rightDelay.getDelay(delayPos); //Delay Time
		rightSample = rightChannel[i] * 0.8 + m_leftDelay.getDelay(delayPos);

		m_leftDelay.writeSample(leftSample * *m_feedback); //Feedback Rate
		m_rightDelay.writeSample(rightSample * *m_feedback);

		leftChannel[i] = leftSample;
		rightChannel[i] = rightSample;

		m_leftDelay.tick();
		m_rightDelay.tick();

		m_phase += 1;
	}
}

double Flanger::getTailLengthSeconds() const
{
	return 0.0;
}

