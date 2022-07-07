/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
struct Placeholder : juce::Component
{
    Placeholder()
    {
        juce::Random r;
        customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
    };
    
    void paint (juce:: Graphics& g) override
    {
        g.fillAll(customColor);
    }
    
    juce:: Colour customColor;
};

/**
*/
class SimpleMBCompAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleMBCompAudioProcessorEditor (SimpleMBCompAudioProcessor&);
    ~SimpleMBCompAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleMBCompAudioProcessor& audioProcessor;
    
    // These are the four different sections of the GUI
    Placeholder controlBar, analyzer, globalControls, bandControls;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleMBCompAudioProcessorEditor)
};
