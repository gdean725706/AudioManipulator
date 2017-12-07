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


//==============================================================================
/*
*/
class XYPadComponent    : public Component, public FlexItem, public Timer
{
public:
    XYPadComponent(int width, int height, EffectChain& effectChain, MainAudioProcessor& processor):
		m_width(width),
		m_height(height),
		FlexItem(width,height),
		m_pointX(0),
		m_pointY(height),
		m_colour(Colours::grey),
		m_currentXY("Default"),
		m_linkedEffectChain(&effectChain),
		m_processor(processor)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		//Set up flex item
		associatedComponent = this;

		setSize(width, height);

		m_xStore.reserve(32000);
		m_yStore.reserve(32000);
		m_pointsSaved = 0;
		m_normalX = 0;
		m_normalY = 0;
		m_playback = false;
		m_playbackCounter = 0;
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
		int invY = m_height - m_pointY;
		return (float)invY / (float)m_height;
	}

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */


        g.fillAll (m_colour);   // clear the background

        g.setColour (Colours::darkgrey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
		
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
		String points;
		points << m_pointsSaved;
		//g.drawText(points, getLocalBounds().removeFromLeft(50),
		//	Justification::centred, true);   // debug points saved
   
	}

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

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

		// Clamp values so we can't exceed bounds of component
		m_pointX = clamp(x, m_width, (double)0.0f);
		m_pointY = clamp(y, m_height, (double) 0.0f);

		m_normalX = getXValueNormalised();
		m_normalY = getYValueNormalised();
		
		m_linkedEffectChain->setXY(m_normalX, m_normalY);
		m_processor.setXY(m_normalX, m_normalY);
	
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

	void writePoints(bool writing)
	{
		if (writing)
		{
			startTimer(timer_rate);
		}
		else
		{
			stopTimer();
			m_xStore.shrink_to_fit();
			m_yStore.shrink_to_fit();
			std::cout << "Saved " << m_pointsSaved << "to path1." << std::endl;
		}
	}

	void startPointPlayback()
	{
		m_playbackCounter = 0;
		m_playback = true;
		startTimer(timer_rate);
	}

	void stopPointPlayback()
	{
		m_playback = false;
		stopTimer();
	}

	void timerCallback() override
	{
		if (m_playback)
		{
			if ( !m_xStore.empty() && !m_yStore.empty() )
			{
				m_playbackX = m_xStore[m_playbackCounter];
				m_playbackY = m_yStore[m_playbackCounter];

				if (m_playbackCounter >= m_pointsSaved - 1) m_playbackCounter = 0;
				else m_playbackCounter++;

				updateXYPoints();
				repaint();
			}
		}
		else
		{
			m_xStore.push_back(m_pointX);
			m_yStore.push_back(m_pointY);
			m_pointsSaved++;
		}
	}

private:
	Colour m_colour;

	int m_width, m_height;
	int m_pointX, m_pointY;
	float m_normalX, m_normalY;
	String m_currentXY;

	EffectChain* m_linkedEffectChain;
	MainAudioProcessor& m_processor;

	// Path writing system
	std::vector<int> m_xStore, m_yStore;
	int m_pointsSaved;

	bool m_playback;
	int m_playbackCounter, m_playbackX, m_playbackY;

	const int timer_rate = 10;

	int scaleRange(int input, int inputStart, int inputEnd, int outputStart, int outputEnd)
	{
		return outputStart + ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart);
	}

	double clamp(double x, double upper, double lower)
	{
		return std::min(upper, std::max(x, lower));
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};
