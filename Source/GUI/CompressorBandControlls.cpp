/*
  ==============================================================================

    CompressorBandControlls.cpp
    Created: 11 Jul 2022 12:45:57pm
    Author:  Thomas Holz

  ==============================================================================
*/

#include "CompressorBandControlls.h"
#include <JuceHeader.h>
#include "../DSP/Params.h"
#include "Utilities.h"

CompressorBandControls::CompressorBandControls(juce::AudioProcessorValueTreeState& apv) :
apvts(apv),
attackSlider(nullptr, "ms", "ATTACK"),
releaseSlider (nullptr, "ms", "RELEASE"),
thresholdSlider(nullptr, "dB", " THRESH"),
ratioSlider(nullptr, "")
{

    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);
    
    bypassButton.addListener(this);
    soloButton.addListener(this);
    muteButton.addListener(this);
    
    bypassButton.setName("X");
    soloButton.setName("S");
    muteButton.setName("M");
    
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(soloButton);
    addAndMakeVisible(muteButton);

    lowBandButton.setName("Lo");
    midBandButton.setName("Mid");
    highBandButton.setName("Hi");
    
    lowBandButton.setRadioGroupId(1);
    midBandButton.setRadioGroupId(1);
    highBandButton.setRadioGroupId(1);
    
    auto buttonSwitcher = [safePtr = this->safePtr]()
    {
        if ( auto* c = safePtr.getComponent() )
        {
            c->updateAttachments();
        }
    };
    
    lowBandButton.onClick = buttonSwitcher;
    midBandButton.onClick = buttonSwitcher;
    highBandButton.onClick = buttonSwitcher;
    
    lowBandButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    updateAttachments();
    
    addAndMakeVisible(lowBandButton);
    addAndMakeVisible(midBandButton);
    addAndMakeVisible(highBandButton);
}

CompressorBandControls::~CompressorBandControls()
{
    bypassButton.removeListener(this);
    soloButton.removeListener(this);
    muteButton.removeListener(this);
}

void CompressorBandControls::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    auto spacer = juce::FlexItem().withWidth(4);
    
    // used for band selection and mute/solo/bypass
    auto createBandButtonControlBox = [](std::vector<Component*> comps)
    {
        juce::FlexBox flexBox;
        flexBox.flexDirection= juce::FlexBox::Direction::column;
        flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
        
        auto spacer = juce::FlexItem().withHeight(2);
        
        for ( auto* comp : comps )
        {
            flexBox.items.add(spacer);
            flexBox.items.add(juce::FlexItem(*comp).withFlex(1.f));
        }
        
        flexBox.items.add(spacer);
        return flexBox;
    };
    
    auto bandButtonControlBox = createBandButtonControlBox({&bypassButton, &soloButton, &muteButton});
    auto bandSelectControlBox = createBandButtonControlBox({&lowBandButton, &midBandButton, &highBandButton});
    
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(bandSelectControlBox).withWidth(50));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(attackSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(releaseSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(thresholdSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(ratioSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(juce::FlexItem(bandButtonControlBox).withWidth(30));
    
    flexBox.performLayout(bounds);
}



void CompressorBandControls::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds();
    drawModuleBackground(g, bounds);
}

void CompressorBandControls::buttonClicked(juce::Button *button)
{
    updateSliderEnablements();
    updateSoloMuteBypassToggleStates(*button);
}
    
void CompressorBandControls::updateSliderEnablements()
{
    auto disabled = muteButton.getToggleState() || bypassButton.getToggleState();
    attackSlider.setEnabled( !disabled );
    releaseSlider.setEnabled( !disabled );
    thresholdSlider.setEnabled( !disabled );
    ratioSlider.setEnabled( !disabled );
}

void CompressorBandControls::updateSoloMuteBypassToggleStates(juce::Button& clickedButton)
{
    if( &clickedButton == &soloButton && soloButton.getToggleState() )
    {
        bypassButton.setToggleState(false, juce::NotificationType::sendNotification);
        muteButton.setToggleState(false, juce::NotificationType::sendNotification);
    }
    else if( &clickedButton == &muteButton && muteButton.getToggleState() )
    {
        bypassButton.setToggleState(false, juce::NotificationType::sendNotification);
        soloButton.setToggleState(false, juce::NotificationType::sendNotification);
    }
    else if( &clickedButton == &bypassButton && bypassButton.getToggleState() )
    {
        muteButton.setToggleState(false, juce::NotificationType::sendNotification);
        soloButton.setToggleState(false, juce::NotificationType::sendNotification);
    }
}

void CompressorBandControls::updateAttachments()
{
    enum BandType
    {
        Low,
        Mid,
        High
    };
    
    BandType bandType = [this]()
    {
        if ( lowBandButton.getToggleState() ){
            return BandType::Low;
        }
        else if ( midBandButton.getToggleState() ){
            return BandType::Mid;
        }
        
        return BandType::High;
    }();
    
    std::vector<Params::Names> names;
    
    switch (bandType)
    {
        case Low:
            names = std::vector<Params::Names>
            {
                Params::Names::Attack_Low_Band,
                Params::Names::Release_Low_Band,
                Params::Names::Threshold_Low_Band,
                Params::Names::Ratio_Low_Band,
                Params::Names::Mute_Low_Band,
                Params::Names::Solo_Low_Band,
                Params::Names::Bypassed_Low_Band
            };
            break;
        case Mid:
            names =
            {
                Params::Names::Attack_Mid_Band,
                Params::Names::Release_Mid_Band,
                Params::Names::Threshold_Mid_Band,
                Params::Names::Ratio_Mid_Band,
                Params::Names::Mute_Mid_Band,
                Params::Names::Solo_Mid_Band,
                Params::Names::Bypassed_Mid_Band
            };
            break;
        case High:
            names =
            {
                Params::Names::Attack_High_Band,
                Params::Names::Release_High_Band,
                Params::Names::Threshold_High_Band,
                Params::Names::Ratio_High_Band,
                Params::Names::Mute_High_Band,
                Params::Names::Solo_High_Band,
                Params::Names::Bypassed_High_Band
            };
            break;
    }
    
    enum Pos
    {
        Attack,
        Release,
        Threshold,
        Ratio,
        Mute,
        Solo,
        Bypass
    };
    
    const auto& params = Params::GetParams();
    auto getParamHelper = [&params, &apvts = this->apvts, names] (const auto& pos)-> auto&
    {
        return getParam(apvts, params, names.at(pos));
    };
    
    attackSliderAttachment.reset();
    releaseSliderAttachment.reset();
    thresholdSliderAttachment.reset();
    ratioSliderAttachment.reset();
    bypassButtonAttachment.reset();
    soloButtonAttachment.reset();
    muteButtonAttachment.reset();
    
    auto& attackParam = getParamHelper(Pos::Attack);
    addLabelPairs(attackSlider.labels, attackParam, "ms");
    attackSlider.changeParam(&attackParam);
    
    auto& releaseParam = getParamHelper(Pos::Release);
    addLabelPairs(releaseSlider.labels, releaseParam, "ms");
    releaseSlider.changeParam(&releaseParam);
    
    auto& threshParam = getParamHelper (Pos::Threshold);
    addLabelPairs(thresholdSlider.labels, threshParam, "dB");
    thresholdSlider.changeParam(&threshParam);
        
    auto& ratioParamRap = getParamHelper(Pos::Ratio);
    ratioSlider.labels.clear();
    ratioSlider.labels.add({0.f, "1:1"});
    auto ratioParam = dynamic_cast<juce::AudioParameterChoice*>(&ratioParamRap);
    ratioSlider.labels.add({1.0f,
                            juce::String(ratioParam->choices.getReference(ratioParam->choices.size() - 1).getIntValue()) + ":1"});
    ratioSlider.changeParam(ratioParam);

    auto makeAttachmentHelper = [&params, &apvts = this->apvts] (auto& attachment,
                                                   const auto& name,
                                                   auto& slider)
    {
        makeAttachment (attachment, apvts, params, name, slider);
    };
    
    makeAttachmentHelper(attackSliderAttachment, names[Pos::Attack], attackSlider);
    makeAttachmentHelper(releaseSliderAttachment, names[Pos::Release], releaseSlider);
    makeAttachmentHelper(thresholdSliderAttachment, names[Pos::Threshold], thresholdSlider);
    makeAttachmentHelper(ratioSliderAttachment, names[Pos::Ratio], ratioSlider);
    makeAttachmentHelper(bypassButtonAttachment, names[ Pos::Bypass], bypassButton);
    makeAttachmentHelper(soloButtonAttachment, names[Pos::Solo], soloButton);
    makeAttachmentHelper(muteButtonAttachment, names[Pos::Mute], muteButton);
}
