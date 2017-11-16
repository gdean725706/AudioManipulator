/*
  ==============================================================================

    FlexSliderComponent.h
    Created: 25 Oct 2017 2:43:45pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FlexSlider    : public Slider, public FlexItem
{
public:
    FlexSlider(String name) :
		Slider(name),
		FlexItem(25, 75)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		associatedComponent = this;
    }

    ~FlexSlider()
    {
    }

    void paint (Graphics& g) override
    {
		g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
		g.fillAll();
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexSlider)
};
