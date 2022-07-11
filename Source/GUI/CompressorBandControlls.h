/*
  ==============================================================================

    CompressorBandControlls.h
    Created: 11 Jul 2022 12:45:57pm
    Author:  Thomas Holz

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RotarySliderWithLabels.h"

struct CompressorBandControls : juce::Component, juce::Button::Listener
{
    CompressorBandControls(juce::AudioProcessorValueTreeState& apvts);
    ~CompressorBandControls() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
private:
    juce::AudioProcessorValueTreeState& apvts;
    RotarySliderWithLabels attackSlider, releaseSlider, thresholdSlider;
    RatioSlider ratioSlider;
    
    // allocate on heap
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> attackSliderAttachment,
                                releaseSliderAttachment,
                                thresholdSliderAttachment,
                                ratioSliderAttachment;
    
    juce::ToggleButton bypassButton, soloButton, muteButton, lowBandButton, midBandButton, highBandButton;
    
    using BtnAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<BtnAttachment>  bypassButtonAttachment,
                                    soloButtonAttachment,
                                    muteButtonAttachment;
    
    juce::Component::SafePointer<CompressorBandControls> safePtr {this};
    void updateAttachments();
    void updateSliderEnablements();
    void updateSoloMuteBypassToggleStates(juce::Button& clickedButton);
};