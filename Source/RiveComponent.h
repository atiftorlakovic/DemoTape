#pragma once

#include <cstdint>  // For uint32_t, etc.
#include <string>   // For std::string

#include <JuceHeader.h>
#include <memory>

// Required Rive includes for implementation
#include <rive/core/binary_reader.hpp>
#include <rive/animation/linear_animation.hpp>
#include <rive/file.hpp>
#include <rive/artboard.hpp>
#include <rive/span.hpp>

#include "RiveOpenGLRenderer.h"

// Forward declarations only needed in header
namespace rive {
    class File;
    class Artboard;
    class Renderer;
    class LinearAnimationInstance; // Added forward decl
}
class RiveOpenGLRenderer; // Forward declare Renderer class

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class RiveComponent : public juce::OpenGLAppComponent
                    , public juce::Button::Listener
{
private:
    // Add a simple factory struct (stub)
    struct MinimalRiveFactory {};
    
    MinimalRiveFactory m_riveFactory;
    juce::OpenGLContext openGLContext;
    std::unique_ptr<rive::File> riveFile;
    std::unique_ptr<rive::Artboard> currentArtboard;
    RiveOpenGLRenderer riveRenderer;
    
    juce::TextButton loadButton;
    bool isPlaying = true;
    double lastTime = 0.0;
    int animationCount = 0;
    int currentAnimationIndex = -1;

public:
    RiveComponent(); // Constructor declaration
    ~RiveComponent() override; // Destructor declaration
    
    void paint(juce::Graphics& g) override; // Declaration only
    
    void resized() override; // Declaration only
    
    // Button listener
    void buttonClicked(juce::Button* button) override; // Declaration only
    
    void play(); // Declaration only
    void pause(); // Declaration only
    void stop(); // Declaration only
    
    int getAnimationCount(); // Declaration only
    
    void setAnimation(int index); // Declaration only
    
    // OpenGL rendering methods
    void newOpenGLContextCreated() override; // Declaration only
    
    void renderOpenGL() override; // Declaration only
    
    void openGLContextClosing() override; // Declaration only

    // Add back declarations for OpenGLAppComponent overrides
    void initialise() override;
    void shutdown() override;
    void render() override;

private:
    // Timer is now an implementation detail
    juce::Timer* animationTimer = nullptr; // Or handle internally

    // Need Timer class declaration for private implementation
    class AnimationTimer : public juce::Timer {
    public:
        AnimationTimer(RiveComponent& comp) : owner(comp) {}
        void timerCallback() override { owner.timerCallback(); }
    private:
        RiveComponent& owner;
    };
    AnimationTimer internalTimer {*this};

    // Private timer callback implementation
    void timerCallback();

    //==============================================================================
    // Member variables
    //==============================================================================
    std::unique_ptr<RiveOpenGLRenderer> renderer;
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RiveComponent)
}; // End of class RiveComponent  