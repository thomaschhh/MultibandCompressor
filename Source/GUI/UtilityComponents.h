/*
  ==============================================================================

    UtilityComponents.h
    Created: 11 Jul 2022 12:23:43pm
    Author:  Thomas Holz

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Placeholder : juce::Component
{
    Placeholder();
    
    void paint (juce:: Graphics& g) override;
    
    juce::Colour customColor;
};

struct RotarySlider : juce::Slider
{
    RotarySlider();
};
