/*
  ==============================================================================

    Utilities.cpp
    Created: 11 Jul 2022 12:40:49pm
    Author:  Thomas Holz

  ==============================================================================
*/

#include "Utilities.h"
#include <JuceHeader.h>

juce::String getValString(const juce::RangedAudioParameter& param,
                          bool getLow,
                          juce::String suffix)
{
    juce::String str;

    auto val = getLow ? param.getNormalisableRange().start :
                        param.getNormalisableRange().end;
    
    bool useK = truncateKiloValue(val);
    str << val;
    
    if ( useK )
    {
        str << "k";
    }
    
    str << suffix;
    return str;
}

void drawModuleBackground(juce::Graphics &g,
                          juce::Rectangle<int> bounds)
{
    g.setColour(juce::Colours::blueviolet);
    g.fillAll();
    
    auto localBounds = bounds;
    
    bounds.reduce(3, 3);
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.drawRect(localBounds);
}
