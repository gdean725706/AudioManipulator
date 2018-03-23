/*
  ==============================================================================

    RecordingBuffer.h
    Created: 11 Feb 2018 8:02:51pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "Phasor.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class RecordingBuffer
{
public:
	RecordingBuffer(float sampleRate = 44100, float bufferSizeSeconds = 5) :
		m_sampleRate(sampleRate),
		m_bufferSize(sampleRate * bufferSizeSeconds),
		m_bufferSizeSeconds(bufferSizeSeconds),
		m_audioBuffer(m_bufferSize),
		m_phasorSpeedMultiplier(1),
		m_phasorSpeed(sampleRate / m_audioBuffer.size()),
		m_playbackActive(false),
		m_filled(false)
	{

	}

	RecordingBuffer(int numberOfSamples) :
		m_bufferSize(numberOfSamples),
		m_audioBuffer(numberOfSamples)
	{
		fillHann();
	}

	void setSampleRate(float sampleRate)
	{
		m_sampleRate = sampleRate;
		m_audioPhasor.setSampleRate(sampleRate);
		calculatePhasorFrequency();
	}

	void fillBuffer(std::vector<float> data, int size)
	{
		m_audioBuffer.clear();
		m_audioBuffer.resize(size - 1);
		m_audioBuffer = data;
		m_bufferSize = size;
		calculatePhasorFrequency();
		m_filled = true;
	}

	float getSample(float index)
	{
		if (index == (float)index)
		{
			double p = index;
			rangeMap(p);

			return m_audioBuffer[p];
		}
		else
		{
			return cubicLookup(index);
		}
	}

	float getNextSampleCubic()
	{
		float r = cubicLookup(m_audioPhasor.getPhase() * m_bufferSize);
		m_audioPhasor.tick();
		return r;
	}

	float getNextSampleLinear()
	{
		float r = linearLookup(m_audioPhasor.getPhase() * m_bufferSize);
		m_audioPhasor.tick();
		return r;
	}

	void clearBuffer()
	{
		m_audioBuffer.clear();
		m_bufferSize = 0;
		m_filled = false;
	}

	void setSpeedMultiplier(float multiplier)
	{
		m_phasorSpeedMultiplier = multiplier;
		calculatePhasorFrequency();
	}

	bool isActive()
	{
		return m_playbackActive;
	}

	void setActive(bool active)
	{
		m_playbackActive = active;
	}

	int getSize()
	{
		if (m_filled)
		{
			return m_audioBuffer.size() - 1;
		}
		else return 0;
	}

	void setSize(int size)
	{
		m_filled = false;
		m_audioBuffer.resize(size);
		m_filled = true;
	}

	template <typename T>
	void rangeMap(T& value)
	{
		int size = m_audioBuffer.size() - 1;

		while (value < 0) value += size;
		while (value > size) value -= size;
	}

	void fillHann(int length = 512)
	{
		clearBuffer();
		m_audioBuffer.resize(length);

		int size = m_audioBuffer.size() - 1;

		for (int i = 0; i < size; ++i)
		{
			m_audioBuffer[i] = 0.5f * (1.0f - cos((6.28318530718 * i) / (float)size));
		}
		m_filled = true;
	}

	void fillSine(int length = 512, float frequency = 1)
	{
		clearBuffer();
		m_audioBuffer.resize(length);

		int size = m_audioBuffer.size() - 1;

		for (int n = 0; n < size; ++n)
		{
			m_audioBuffer[n] = sinf((6.28318530718 * n * frequency) / size);
		}

		m_filled = true;
	}

private:

	template <typename T>
	T linearLookup(T index)
	{
		T aIndex, bIndex, r;
		r = modf(index, &aIndex);
		bIndex = aIndex + 1;
		rangeMap(aIndex);
		rangeMap(bIndex);
		return lerp(m_audioBuffer[aIndex], m_audioBuffer[bIndex], r);
	}

	template <typename T>
	T cubicLookup(T index)
	{
		T lower, remainder;
		remainder = modf(index, &lower);
		T y0 = lower - 1;
		T y1 = lower;
		T y2 = lower + 1;
		T y3 = lower + 2;

		rangeMap(y0);
		rangeMap(y1);
		rangeMap(y2);
		rangeMap(y3);

		T y0Val(m_audioBuffer[y0]), y1Val(m_audioBuffer[y1]), y2Val(m_audioBuffer[y2]), y3Val(m_audioBuffer[y3]);
		return cubicInterpolate(y0Val, y1Val, y2Val, y3Val, remainder);
	}



	// Recalculate phasor frequency
	void calculatePhasorFrequency()
	{
		m_phasorSpeed = m_sampleRate / m_bufferSize;
		m_phasorSpeed *= m_phasorSpeedMultiplier;
		m_audioPhasor.setFrequency(m_phasorSpeed);


		String s = ("Speed = ");
		s << m_phasorSpeed;
		String s1 = "Mult = ";
		s1 << m_phasorSpeedMultiplier;
		DBG(s);
		DBG(s1);
	}

	float m_sampleRate, m_bufferSize, m_bufferSizeSeconds;
	Phasor m_audioPhasor;
	std::vector<float> m_audioBuffer;
	float m_phasorSpeedMultiplier, m_phasorSpeed;
	bool m_playbackActive;
	bool m_filled;

};

//https://devblogs.nvidia.com/lerp-faster-cuda/
template <class T>
T lerp(T v0, T v1, T t)
{
	return fma(t, v1, fma(-t, v0, v0));
}

//http://paulbourke.net/miscellaneous/interpolation/
template <class T>
T cubicInterpolate(
	T y0, T y1,
	T y2, T y3,
	T mu)
{
	T a0, a1, a2, a3, mu2;

	mu2 = mu*mu;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;

	return(a0*mu*mu2 + a1*mu2 + a2*mu + a3);
}

// Rangemap for audio signals
// same as pd wrap~
template <class T>
T wrap(T value)
{
	while (value > 1.0)
	{
		value -= 1;
	}
	while (value < 0.0)
	{
		value += 1;
	}

	return value;
}
