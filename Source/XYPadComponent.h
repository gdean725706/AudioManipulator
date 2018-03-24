/*
  ==============================================================================

    XYPadComponent.h
    Created: 10 Oct 2017 1:11:23pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"
#include <vector>
#include "RecordingBuffer.h"


//==============================================================================
/*
*/
class XYPadComponent    : public Component, public FlexItem, public Timer
{
public:

static enum InterpolationMode
{
	None,
	Cubic,
	Linear
};

    XYPadComponent(int width, int height, EffectChain* effectChain, MainAudioProcessor* processor):
		m_width(width),
		m_height(height),
		FlexItem(width,height),
		m_pointX(0),
		m_pointY(height),
		m_colour(Colours::grey),
		m_currentXY("Default"),
		m_linkedEffectChain(effectChain),
		m_processor(processor),
		m_savedBuffersX(3),
		m_savedBuffersY(3),
		m_playbackX(0),
		m_playbackY(0),
		m_bufferInterpolation(0.0f)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		//Set up flex item
		associatedComponent = this;
		flexGrow = 1.0f;
		flexShrink = 1.0f;

		for (int i = 0; i < 3; ++i)
		{
			m_savedBuffersX[i].setSampleRate(timer_rate_Hz);
			m_savedBuffersY[i].setSampleRate(timer_rate_Hz);
		}

		m_bufferX.reserve(32000);
		m_bufferY.reserve(32000);
		m_writingToBuffer = false;
		m_pointsSaved = 0;
		m_normalX = 0;
		m_normalY = 0;
		m_playback = false;
		
    }

    ~XYPadComponent()
    {
    }

	// Getters for X & Y values
	// Return value normalised to a specified range
	int getXValue(int minScaleValue, int maxScaleValue)
	{
		return scaleRange(m_pointX, 0, m_width, minScaleValue, maxScaleValue);
	}

	float getXValueNormalised()
	{
		return (float)m_pointX / (float)m_width;
	}

	int getYValue(int minScaleValue, int maxScaleValue)
	{
		int invertedY = m_height - m_pointY;
		return scaleRange(invertedY, 0, m_height, minScaleValue, maxScaleValue);
	}
	float getYValueNormalised()
	{
		// Invert height coordinate to start from bottom left corner.
		int invY = m_height - m_pointY;
		return (float)invY / (float)m_height;
	}

    void paint (Graphics& g) override
    { 
		// clear the background
        g.fillAll (m_colour);  

        g.setColour (Colours::darkgrey);

		// draw an outline around the component
        g.drawRect (getLocalBounds(), 1);   
		
		// Update string with XY coords
		updateMouseXYText();

		// Horizontal Line
		g.drawRect(m_pointX, 0, 2, this->getHeight(), 1);
		// Vertical line
		g.drawRect(0, m_pointY, this->getWidth(), 2, 1);

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText (m_currentXY, getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
		//String points;
		//points << m_pointsSaved;
		//g.drawText(points, getLocalBounds().removeFromLeft(50),
		//	Justification::centred, true);   // debug points saved
   
	}

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		m_width = getWidth();
		m_height = getHeight();

    }

	// Update XY point when mouse clicked
	void mouseDown(const MouseEvent& event) override
	{
		updateXYPoints();

		repaint();
	}

	// Allow for dragging XY input
	void mouseDrag(const MouseEvent& event) override
	{
		updateXYPoints();

		repaint();		
	}

	void updateXYPoints()
	{
		int x = 0, y = 0;

		if (m_playback)
		{
			x = m_playbackX;
			y = m_playbackY;
		}
		else
		{
			// get mouse point relative to this component
			auto mousePoint = this->getMouseXYRelative();
			x = mousePoint.x;
			y = mousePoint.y;
		}

		// Clamp values for safety so we can't exceed bounds of component
		m_pointX = clamp(x, m_width, 0.0);
		m_pointY = clamp(y, m_height, 0.0);

		// Update normalised values
		m_normalX = getXValueNormalised();
		m_normalY = getYValueNormalised();
		
		// Pass back normalised values to processors
		m_linkedEffectChain->setXY(m_normalX, m_normalY);
		m_processor->setXY(m_normalX, m_normalY);
	
	}

	// Update string containing mouse XY values
	void updateMouseXYText()
	{
		// Dynamic casting stream int to string
		String xText, yText;
		xText << m_pointX;
		yText << m_pointY;
		
		m_currentXY = xText + "," + yText;
	}

	void startRecordingPoints(int index)
	{
		if (!m_writingToBuffer)
		{
			m_pointsSaved = 0;
			m_bufferX.clear();
			m_bufferY.clear();
			m_writingToBuffer = true;
			startTimerHz(timer_rate_Hz);
		}
	}

	void stopRecordingPoints(int index)
	{
		index = index > 3 ? 3 : index < 0 ? 0 : index;

		m_savedBuffersX[index].fillBuffer(m_bufferX, m_pointsSaved);
		m_savedBuffersY[index].fillBuffer(m_bufferY, m_pointsSaved);

		m_bufferX.clear();
		m_bufferY.clear();
		m_pointsSaved = 0;
		m_writingToBuffer = false;
	}


	void startPointPlayback(int index)
	{
		m_savedBuffersX[index].setActive(true);
		m_savedBuffersY[index].setActive(true);
		m_playback = true;
		startTimerHz(timer_rate_Hz);
	}

	void stopPointPlayback(int index)
	{
		m_savedBuffersX[index].setActive(false);
		m_savedBuffersY[index].setActive(false);
		m_playback = false;
		stopTimer();
	}

	void setPlaybackRate(float rate)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_savedBuffersX[i].setSpeedMultiplier(rate);
			m_savedBuffersY[i].setSpeedMultiplier(rate);
		}
	}

	void timerCallback() override
	{
		if (m_playback)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (m_savedBuffersX[i].isActive() == true)
				{
					if (m_currentInterpolationMode == InterpolationMode::Cubic)
						m_playbackX = m_savedBuffersX[i].getNextSampleCubic();
					else
						m_playbackX = m_savedBuffersX[i].getNextSampleLinear();
				}

				if (m_savedBuffersY[i].isActive() == true)
				{
					if (m_currentInterpolationMode == InterpolationMode::Cubic)
						m_playbackY = m_savedBuffersY[i].getNextSampleCubic();
					else
						m_playbackY = m_savedBuffersY[i].getNextSampleLinear();
				}
			}

			updateXYPoints();
			repaint();

		}
		else if (m_writingToBuffer)
		{
			m_bufferX.push_back(m_pointX);
			m_bufferY.push_back(m_pointY);
			m_pointsSaved++;
		}
	}

	void setInterpolationMode(InterpolationMode mode)
	{
		m_currentInterpolationMode = mode;
	}

	InterpolationMode getInterpolationMode()
	{
		return m_currentInterpolationMode;
	}


private:
	Colour m_colour;

	int m_width, m_height;
	int m_pointX, m_pointY;
	float m_normalX, m_normalY;
	String m_currentXY;

	EffectChain* m_linkedEffectChain;
	MainAudioProcessor* m_processor;

	// Path writing system
	std::vector<float> m_bufferX, m_bufferY;
	bool m_writingToBuffer;
	int m_pointsSaved;

	std::vector<Wavetable> m_savedBuffersX, m_savedBuffersY;
	int m_numberOfBuffers;
	bool m_playback;
	int m_playbackX, m_playbackY;
	float m_bufferInterpolation;

	const int timer_rate_Hz = 60;

	InterpolationMode m_currentInterpolationMode = InterpolationMode::Cubic;

	double clamp(double x, double upper, double lower)
	{
		return std::min(upper, std::max(x, lower));
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};
