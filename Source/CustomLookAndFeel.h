/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 28 Mar 2018 6:37:54pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V4
{
	String m_customText;
	Justification m_customTextJustification;

public:
	CustomLookAndFeel(String customText = "Custom", Justification just = Justification::centredTop) :
		m_customText(customText),
		m_customTextJustification(just)
	{

	}

	void setJustification(Justification just)
	{
		m_customTextJustification = just;
	}

	void setCustomText(String text)
	{
		m_customText = text;
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		const auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		const auto fill = slider.findColour(Slider::rotarySliderFillColourId);

		const auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

		auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
		const auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto lineW = jmin(8.0f, radius * 0.5f);
		auto arcRadius = radius - lineW * 0.5f;

		Path backgroundArc;
		backgroundArc.addCentredArc(bounds.getCentreX(),
			bounds.getCentreY(),
			arcRadius,
			arcRadius,
			0.0f,
			rotaryStartAngle,
			rotaryEndAngle,
			true);

		g.setColour(outline);
		g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

		if (slider.isEnabled())
		{
			Path valueArc;
			valueArc.addCentredArc(bounds.getCentreX(),
				bounds.getCentreY(),
				arcRadius,
				arcRadius,
				0.0f,
				rotaryStartAngle,
				toAngle,
				true);

			g.setColour(fill);
			g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
		}

		const auto thumbWidth = lineW * 2.0f;
		const Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - float_Pi * 0.5f),
			bounds.getCentreY() + arcRadius * std::sin(toAngle - float_Pi * 0.5f));

		g.setColour(slider.findColour(Slider::thumbColourId));
		g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));

		g.setColour(Colours::lightgrey);
		g.setFont(12);
		g.drawText(m_customText, bounds, m_customTextJustification, false);
	}

};