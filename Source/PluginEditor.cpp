/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DemoTapeAudioProcessorEditor::DemoTapeAudioProcessorEditor(DemoTapeAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be
    setSize(800, 600);
    
    // Add Rive component
    addAndMakeVisible(riveComponent);
    
    // Setup UI controls
    addAndMakeVisible(loadRiveFileButton);
    addAndMakeVisible(animationSelector);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
    
    // Setup button callbacks
    loadRiveFileButton.onClick = [this] { loadRiveFile(); };
    playButton.onClick = [this] { riveComponent.play(); };
    pauseButton.onClick = [this] { riveComponent.pause(); };
    stopButton.onClick = [this] { riveComponent.stop(); };
    
    animationSelector.onChange = [this] {
        riveComponent.setAnimation(animationSelector.getSelectedItemIndex());
    };
}

DemoTapeAudioProcessorEditor::~DemoTapeAudioProcessorEditor()
{
}

void DemoTapeAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void DemoTapeAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto controlsHeight = 30;
    auto controlsArea = bounds.removeFromBottom(controlsHeight * 2);
    
    // Position Rive component in the main area
    riveComponent.setBounds(bounds);
    
    // Position controls in the bottom area
    auto row1 = controlsArea.removeFromTop(controlsHeight);
    loadRiveFileButton.setBounds(row1.removeFromLeft(150));
    animationSelector.setBounds(row1);
    
    auto row2 = controlsArea;
    auto buttonWidth = row2.getWidth() / 3;
    playButton.setBounds(row2.removeFromLeft(buttonWidth));
    pauseButton.setBounds(row2.removeFromLeft(buttonWidth));
    stopButton.setBounds(row2);
}

void DemoTapeAudioProcessorEditor::loadRiveFile()
{
    // NOTE: File loading is now handled by the button INSIDE RiveComponent.
    // This external button and logic are redundant if the internal button is used.
    // If you want this external button, it should trigger the internal one, perhaps?
    // For now, commenting out the file choosing logic here.
    
    // juce::Logger::writeToLog("PluginEditor::loadRiveFile() called - This button should likely trigger RiveComponent's internal button/logic.");

    /* // OLD LOGIC - RiveComponent now handles its own file loading via its button.
    // Create file chooser dialog
    auto fileChooser = std::make_shared<juce::FileChooser>(
        "Choose a Rive File",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.riv");
    
    auto fileChooserFlags = 
        juce::FileBrowserComponent::openMode | 
        juce::FileBrowserComponent::canSelectFiles;
    
    fileChooser->launchAsync(fileChooserFlags, [this, fileChooser](const juce::FileChooser& fc) {
        if (fc.getResults().size() > 0)
        {
            juce::File riveFile = fc.getResult();
            
            // if (riveComponent.loadRiveFile(riveFile)) // loadRiveFile removed from RiveComponent public API
            // Trigger the internal load button or a new public load method if needed
            juce::Logger::writeToLog("Need to trigger load in RiveComponent for file: " + riveFile.getFullPathName());
            
            // {
            //    setupAnimationControls();
            // }
            // else
            // {
            //    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            //                                          "Error",
            //                                          "Failed to load Rive file: " + riveFile.getFileName());
            // }
        }
    });
    */
}

void DemoTapeAudioProcessorEditor::setupAnimationControls()
{
    // NOTE: Animation control logic (getAnimationCount, setAnimation) was moved 
    // to be internal to RiveComponent or handled via RiveOpenGLRenderer.
    // This setup logic needs to be adapted or removed if RiveComponent manages its state.
    juce::Logger::writeToLog("PluginEditor::setupAnimationControls() called - Needs update for current RiveComponent structure.");

    /* // OLD LOGIC - Requires getAnimationCount/setAnimation on RiveComponent
    // Clear existing items
    animationSelector.clear();
    
    // Get the number of animations
    // int numAnimations = riveComponent.getAnimationCount(); // Method removed/made private
    int numAnimations = 0; // Placeholder
    
    if (numAnimations > 0)
    {
        // Add each animation to the selector
        for (int i = 0; i < numAnimations; ++i)
        {
            animationSelector.addItem("Animation " + juce::String(i + 1), i + 1);
        }
        
        // Select the first animation
        animationSelector.setSelectedItemIndex(0);
    }
    else
    {
        // No animations available
        animationSelector.addItem("No animations", 1);
        animationSelector.setEnabled(false);
    }
    */
}
 