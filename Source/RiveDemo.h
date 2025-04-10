#pragma once

#include "RiveComponent.h"
#include <JuceHeader.h>

class RiveDemo : public RiveComponent
{
public:
    RiveDemo()
    {
        // Set up demo controls
        addAndMakeVisible(exampleSelector);
        exampleSelector.addItem("Simple Animation", 1);
        exampleSelector.addItem("Interactive Button", 2);
        exampleSelector.addItem("Loading Spinner", 3);
        exampleSelector.addItem("Toggle Switch", 4);
        exampleSelector.addItem("Progress Bar", 5);
        
        exampleSelector.onChange = [this] {
            loadSelectedExample();
        };
        
        exampleSelector.setSelectedItemIndex(0);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        auto selectorHeight = 30;
        exampleSelector.setBounds(bounds.removeFromTop(selectorHeight));
        
        // Let the base component handle the rest
        RiveComponent::resized();
    }
    
    void loadSelectedExample()
    {
        int selected = exampleSelector.getSelectedItemIndex();
        
        // Path to demo files (embedded or in the resources folder)
        juce::String filename;
        
        switch (selected)
        {
            case 0: filename = "simple_animation.riv"; break;
            case 1: filename = "button.riv"; break;
            case 2: filename = "spinner.riv"; break;
            case 3: filename = "toggle.riv"; break;
            case 4: filename = "progress.riv"; break;
            default: filename = "simple_animation.riv";
        }
        
        // Try to load from local resources folder
        juce::File resourceDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                                .getParentDirectory()
                                .getChildFile("Resources");
                                
        if (!resourceDir.exists())
            resourceDir = juce::File::getCurrentWorkingDirectory().getChildFile("Resources");
        
        juce::File riveFile = resourceDir.getChildFile(filename);
        
        if (riveFile.existsAsFile())
        {
            bool loaded = loadRiveFile(riveFile);
            if (loaded)
            {
                play(); // Auto-play the animation
            }
            else
            {
                // Failed to load
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                      "Error",
                                                      "Failed to load demo file: " + filename);
            }
        }
        else
        {
            // File not found
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Error",
                                                  "Demo file not found: " + filename + 
                                                  "\nLooking in: " + resourceDir.getFullPathName());
        }
    }
    
private:
    juce::ComboBox exampleSelector;
}; 