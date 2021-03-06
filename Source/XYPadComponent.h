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


class LFO
{
	Wavetable m_lfoOsc;
	Phasor m_phasor;
	float m_xMod, m_yMod;
	bool m_active;
public:
	LFO(float sampleRate = 44100.0f, float frequency = 1.0f) :
		m_lfoOsc(512),
		m_phasor(sampleRate, frequency),
		m_xMod(0),
		m_yMod(0)
	{
		m_lfoOsc.setSampleRate(sampleRate);
		m_lfoOsc.fillSine();
		m_phasor.setSampleRate(sampleRate);
	}

	float getNextSample()
	{
		float s = m_lfoOsc.getSample(m_phasor.getPhase() * m_lfoOsc.getSize());
		m_phasor.tick();
		return s;
	}

	float getSample()
	{
		return m_lfoOsc.getSample(m_phasor.getPhase() * m_lfoOsc.getSize());
	}

	void tick()
	{
		m_phasor.tick();
	}

	void setSampleRate(float sampleRate)
	{
		m_phasor.setSampleRate(sampleRate);
	}

	void setFrequency(float frequency)
	{
		m_phasor.setFrequency(frequency);
	}
	
	bool isActive()
	{
		return m_active;
	}

	void setActive(bool active)
	{
		m_active = active;
	}
};

//==============================================================================
/*
*/
class XYPadComponent    : public AnimatedAppComponent, public FlexItem
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
		m_lastX(0),
		m_lastY(height),
		m_colour(Colours::grey),
		m_currentXY("Default"),
		m_linkedEffectChain(effectChain),
		m_processor(processor),
		m_savedBuffersX(3),
		m_savedBuffersY(3),
		m_playbackX(0),
		m_playbackY(0),
		m_bufferInterpolation(0.0f),
		m_lfoMod(true),
		m_lfoAmpX(3),
		m_lfoAmpY(3),
		m_LFOs(3),
		m_trail(30),
		m_trailIndex(0),
		m_xParamName("XParam"),
		m_yParamName("YParam")
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		for (int i = 0; i < 3; ++i)
		{
			m_LFOs[i].setSampleRate(timer_rate_Hz);
			m_LFOs[i].setFrequency(1.0f);
			m_lfoAmpX[i] = 0.0f;
			m_lfoAmpY[i] = 0.0f;
		}

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

		setFramesPerSecond(60);
    }

    ~XYPadComponent()
    {
    }

	void update() override
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
			m_bufferX.push_back(m_lastX);
			m_bufferY.push_back(m_lastY);
			m_pointsSaved++;
		}

		int lfosActive = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (m_LFOs[i].isActive())
			{
				lfosActive += 1;
			}
		}
		if (lfosActive != 0)
		{
			modifyXY();

		}

		// Trail
		m_trailIndex %= m_trail.size();
		m_trail[m_trailIndex].setXY(m_pointX, m_pointY);
		m_trailIndex++;
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

		m_linkedEffectChain->getLatestNames(m_xParamName, m_yParamName);

		// Horizontal Line

		int xTextPos(0), yTextPos(0);
		float on(0);

		for (int i = 0; i < 3; ++i)
		{
			on += m_lfoAmpX[i];
			on += m_lfoAmpY[i];
		}

		// Detach the text when LFOs activate
		xTextPos = on != 0.0f ? m_width - 200 : m_pointX;
		yTextPos = on != 0.0f ? 0 : m_pointY;
		
		g.drawMultiLineText(m_xParamName, xTextPos + 4, 13, 200);
		g.drawMultiLineText(m_yParamName, 2, yTextPos + 15, 200);

		g.drawRect(m_pointX, 0, 2, this->getHeight(), 1);
		// Vertical line
		g.drawRect(0, m_pointY, this->getWidth(), 2, 1);

		// Draw trail
		//g.setColour(Colours::aliceblue);
		//const int trailLength = m_trail.size();
		//Path trailPath;

		//for (int i = 0; i < trailLength; ++i)
		//{
		//	Point<float> p = m_trail[i];
		//	//Point<float> p(m_pointX + i *radius * std::sin(getFrameCounter() * 0.02f + i) , m_pointY + i *radius * std::cos(getFrameCounter() * 0.02f + i));

		//	//g.fillEllipse(p.x - i, p.y - i, 1.0f + 1.0f * i, 1.0f + 1.0f * i);

		//	if (i == 0)
		//		trailPath.startNewSubPath(p);
		//	else
		//		trailPath.lineTo(p);
		//}

		//g.strokePath(trailPath, PathStrokeType(4.0f));
		//g.setColour(getLookAndFeel().findColour(Slider::backgroundColourId));
		//g.fillPath(trailPath);

        g.setFont (14.0f);
        g.drawText (m_currentXY, getLocalBounds(),
                    Justification::centred, true);
   
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
	}

	void stopPointPlayback(int index)
	{
		m_savedBuffersX[index].setActive(false);
		m_savedBuffersY[index].setActive(false);
		m_playback = false;
		
	}

	void setPlaybackRate(float rate)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_savedBuffersX[i].setSpeedMultiplier(rate);
			m_savedBuffersY[i].setSpeedMultiplier(rate);
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

	void forceXYUpdate(float x, float y)
	{
		m_pointX = x * m_width;
		m_pointY = (1-y) * m_height;
	}

	void setLfoMod(bool active)
	{
		m_lfoMod = active;
	}

	void setLfoAmplitude(float amplitude, int index, int axis)
	{
		if (axis == 0)
		{
			m_lfoAmpX[index] = amplitude;
		}
		else if (axis == 1)
		{
			m_lfoAmpY[index] = amplitude;
		}

		DBG("Setting active lfo " + (String)index);

		m_LFOs[index].setActive(amplitude >= 0.0f);
	}

	void setLfoFrequency(float frequency, int index)
	{

		m_LFOs[index].setFrequency(frequency);

		m_LFOs[index].setActive(frequency >= 0.0f);

		DBG("Setting freq of lfo " + (String)index);
	}

	void setXYStringName(String x, String y)
	{
		m_xParamName = x;
		m_yParamName = y;
	}

private:

	void updateXYPoints()
	{
		int x = 0, y = 0;

		if (m_playback)
		{
			x = m_playbackX;
			y = m_playbackY;
			m_lastX = x;
			m_lastY = y;
		}
		else
		{
			// get mouse point relative to this component
			auto mousePoint = this->getMouseXYRelative();
			x = mousePoint.x;
			y = mousePoint.y;
			m_lastX = x;
			m_lastY = y;
		}


		// Clamp values for safety so we can't exceed bounds of component
		m_pointX = clamp(x, m_width, 0.0);
		m_pointY = clamp(y, m_height, 0.0);

		finalise();

	}

	void finalise()
	{
		// Clamp values again for safety 
		m_pointX = clamp(m_pointX, m_width, 0.0);
		m_pointY = clamp(m_pointY, m_height, 0.0);

		// Update normalised values
		m_normalX = getXValueNormalised();
		m_normalY = getYValueNormalised();

		// Pass back normalised values to processors
		m_linkedEffectChain->setXY(m_normalX, m_normalY);
		m_processor->setXY(m_normalX, m_normalY);
	}

	void modifyXY()
	{

		float totalPointsX = 0, totalPointsY = 0;

		if (m_lfoMod)
		{

			for (int i = 0; i < 3; ++i)
			{
				if (m_LFOs[i].isActive())
				{
					totalPointsX += (m_LFOs[i].getSample() * (m_lfoAmpX[i] * (m_width * 0.5f)));
					totalPointsY += (m_LFOs[i].getSample() * (m_lfoAmpY[i] * (m_height * 0.5f)));
					m_LFOs[i].tick();
				}
			}
			//DBG(totalPointsX);
			//DBG(totalPointsY);

			m_pointX = m_lastX + totalPointsX;
			m_pointY = m_lastY + totalPointsY;

		}

		//DBG(m_LFOs[0].getSample());

		finalise();
		repaint();
	}


	Colour m_colour;

	int m_width, m_height;
	int m_pointX, m_pointY, m_lastX, m_lastY;
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

	bool m_lfoMod;
	std::vector<float> m_lfoAmpX, m_lfoAmpY;
	std::vector<LFO> m_LFOs;


	std::vector<Point<float>> m_trail;
	int m_trailIndex;

	String m_xParamName, m_yParamName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPadComponent)
};

