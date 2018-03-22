/*
  ==============================================================================

    FlexContainer.h
    Created: 22 Mar 2018 12:06:46pm
    Author:  deano

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FlexContainer    : public Component, public FlexBox, public FlexItem
{
public:
    FlexContainer()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

	void resized() override
	{
		auto bounds = getLocalBounds();

		performLayout(bounds);
		// This method is where you should set the bounds of any child
		// components that your component contains..

	}

    ~FlexContainer()
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexContainer)
};
