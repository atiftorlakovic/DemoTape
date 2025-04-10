/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <rive/file.hpp>
#include <rive/artboard.hpp>
#include <rive/renderer.hpp>
#include "RiveComponent.h"

//==============================================================================
/**
*/
class DemoTapeAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DemoTapeAudioProcessorEditor(DemoTapeAudioProcessor&);
    ~DemoTapeAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DemoTapeAudioProcessor& audioProcessor;
    
    // Rive component for UI animations
    RiveComponent riveComponent;
    
    // UI controls
    juce::TextButton loadRiveFileButton {"Load Rive File"};
    juce::ComboBox animationSelector;
    juce::TextButton playButton {"Play"};
    juce::TextButton pauseButton {"Pause"};
    juce::TextButton stopButton {"Stop"};
    
    // Add methods for loading Rive files
    void loadRiveFile();
    void setupAnimationControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoTapeAudioProcessorEditor)
};
