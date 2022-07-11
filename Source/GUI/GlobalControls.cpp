/*
  ==============================================================================

    GlobalControls.cpp
    Created: 11 Jul 2022 12:48:56pm
    Author:  Thomas Holz

  ==============================================================================
*/

#include "GlobalControls.h"
#include "../DSP/Params.h"
#include "Utilities.h"

GlobalControls::GlobalControls(juce::AudioProcessorValueTreeState& apvts)
{
    const auto& params = Params::GetParams();
    
    auto getParamHelper = [&params, &apvts] (const auto& name)-> auto&
    {
        return getParam(apvts, params, name);
    };
    
    auto& gainInParam = getParamHelper(Params::Names::Gain_In);
    auto& gainOutParam = getParamHelper(Params::Names::Gain_Out);
    auto& lowMidParam = getParamHelper(Params::Names::Low_Mid_Crossover_Freq);
    auto& midHighParam = getParamHelper(Params::Names::Mid_High_Crossover_Freq);
    
    inGainSlider = std::make_unique<RSWL>(&gainInParam, "dB", "Input Gain");
    outGainSlider = std::make_unique<RSWL>(&gainOutParam, "dB", "Output Gain");
    lowMidXoverSlider = std::make_unique<RSWL>(&lowMidParam, "Hz", "Low-Mid X Freq.");
    midHighXoverSlider = std::make_unique<RSWL>(&midHighParam, "Hz", "Mid-High X Freq");
    
    
    auto makeAttachmentHelper = [&params, &apvts] (auto& attachment,
                                                   const auto& name,
                                                   auto& slider)
    {
        makeAttachment (attachment, apvts, params, name, slider);
    };
    

    makeAttachmentHelper(inGainSliderAttachment,
                         Params::Names::Gain_In,
                         *inGainSlider);
    makeAttachmentHelper(lowMidXoverSliderAttachment,
                         Params::Names::Low_Mid_Crossover_Freq,
                         *lowMidXoverSlider);
    makeAttachmentHelper(midHighXoverSliderAttachment,
                         Params::Names::Mid_High_Crossover_Freq,
                         *midHighXoverSlider);
    makeAttachmentHelper(outGainSliderAttachment,
                         Params::Names::Gain_Out,
                         *outGainSlider);
    
    addLabelPairs(inGainSlider->labels,
                  gainInParam,
                  "dB");
    addLabelPairs(lowMidXoverSlider->labels,
                  lowMidParam,
                  "Hz");
    addLabelPairs(midHighXoverSlider->labels,
                  midHighParam,
                  "Hz");
    addLabelPairs(outGainSlider->labels,
                  gainOutParam,
                  "dB");
                  
    addAndMakeVisible(*inGainSlider);
    addAndMakeVisible(*lowMidXoverSlider);
    addAndMakeVisible(*midHighXoverSlider);
    addAndMakeVisible(*outGainSlider);
}

void GlobalControls::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds();
    drawModuleBackground(g, bounds);
}
    
void GlobalControls::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    auto spacer = juce::FlexItem().withWidth(4);
    auto endCap= juce::FlexItem().withWidth(6);
    
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBox.items.add(endCap);
    flexBox.items.add(juce::FlexItem(*inGainSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(*lowMidXoverSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(*midHighXoverSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(*outGainSlider).withFlex(1.f));
    flexBox.items.add(endCap);
    
    flexBox.performLayout(bounds);
}
