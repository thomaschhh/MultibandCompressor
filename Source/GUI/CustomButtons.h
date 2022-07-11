/*
  ==============================================================================

    CustomButtons.h
    Created: 11 Jul 2022 12:21:29pm
    Author:  Thomas Holz

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct PowerButton : juce::ToggleButton { };

struct AnalyzerButton : juce::ToggleButton
{
    void resized() override;
    
    juce::Path randomPath;
};
