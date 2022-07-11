/*
  ==============================================================================

    UtilityComponents.cpp
    Created: 11 Jul 2022 12:23:43pm
    Author:  Thomas Holz

  ==============================================================================
*/

#include "UtilityComponents.h"
#include <JuceHeader.h>

Placeholder::Placeholder()
{
    juce::Random r;
    customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
};

void Placeholder::paint(juce::Graphics& g)
{
    g.fillAll(customColor);
}

//==============================================================================

RotarySlider::RotarySlider() :  juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                juce::Slider::TextEntryBoxPosition::NoTextBox)
{ }
