/*
  ==============================================================================

    FlexComboBox.h
    Created: 22 Mar 2018 2:26:21pm
    Author:  George Dean

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FlexComboBox    : public ComboBox, public FlexItem
{
public:
    FlexComboBox(String name, int width = 100, int height = 50) :
		ComboBox(name),
		FlexItem(width, height)
    {
		associatedComponent = this;
    }

    ~FlexComboBox()
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexComboBox)
};
