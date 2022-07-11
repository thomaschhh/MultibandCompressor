/*
  ==============================================================================

    GlobalControls.h
    Created: 11 Jul 2022 12:48:56pm
    Author:  Thomas Holz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RotarySliderWithLabels.h"

struct GlobalControls : juce::Component
{
    // apvts to look up the attachments
    GlobalControls(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    using RSWL = RotarySliderWithLabels;
    std::unique_ptr<RSWL> inGainSlider, lowMidXoverSlider, midHighXoverSlider, outGainSlider;
    
    // allocate on heap
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> lowMidXoverSliderAttachment,
                                midHighXoverSliderAttachment,
                                inGainSliderAttachment,
                                outGainSliderAttachment;
};
