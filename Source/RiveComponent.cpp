#include "RiveComponent.h"
#include "RiveOpenGLRenderer.h"

//==============================================================================
RiveComponent::RiveComponent()
{
    // Add the load button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load Rive File...");
    loadButton.addListener(this);

    // Initialize OpenGL context attachment
    // openGLContext.setRenderer(this); // REMOVE THIS - Renderer not created yet & incorrect cast
    openGLContext.setComponentPaintingEnabled(false); // Disable JUCE painting as OpenGL handles it
    openGLContext.attachTo(*this); // Attach context to this component

    setSize (600, 400);

    // Start a timer for animation updates (e.g., 60 FPS)
    // lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Initialize lastTime
    // startTimerHz(60); // Start timer automatically only if playing
}

RiveComponent::~RiveComponent()
{
    // stopTimer(); // Ensure timer is stopped
    internalTimer.stopTimer(); // Call on the internal timer object
    // Detach context before destruction
    openGLContext.detach();
    renderer = nullptr; // Release the renderer explicitly before context goes away
}

//==============================================================================
// These methods are part of the OpenGLAppComponent interface but are usually
// delegated to the attached OpenGLRenderer (RiveOpenGLRenderer in this case).
// We keep them here for the initial setup but RiveOpenGLRenderer handles the real work.
/* REMOVING these as they delegate to the attached renderer via the context
void RiveComponent::initialise()
{
    juce::Logger::writeToLog("RiveComponent::initialise() called - delegating to renderer");
    // This is called when the context attaches. We create the actual renderer here.
    if (renderer == nullptr)
        renderer = std::make_unique<RiveOpenGLRenderer>();

    // Note: RiveOpenGLRenderer::newOpenGLContextCreated() will be called by the context
}

void RiveComponent::shutdown()
{
     juce::Logger::writeToLog("RiveComponent::shutdown() called - delegating to renderer");
    // Note: RiveOpenGLRenderer::openGLContextClosing() will be called by the context
    renderer = nullptr; // Release the renderer
}

void RiveComponent::render()
{
     juce::Logger::writeToLog("RiveComponent::render() called - NO LONGER USED directly");
    // This method from OpenGLAppComponent is NOT the one called frequently for rendering.
    // The attached OpenGLRenderer's renderOpenGL() method is called by the context.
}
*/

//==============================================================================
void RiveComponent::paint (juce::Graphics& g)
{
    // You generally don't draw traditional JUCE graphics
    // when using an OpenGL context directly. The 'renderOpenGL()' method of the
    // attached RiveOpenGLRenderer handles drawing.
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); // Clear if needed? Probably not.
}

void RiveComponent::resized()
{
    // Position the load button
    loadButton.setBounds(10, 10, 150, 24);

    // The OpenGL context resizes automatically with the component.
    // RiveOpenGLRenderer::renderOpenGL() should use the current context size.
}

void RiveComponent::timerCallback()
{
    // Only advance animation if playing and the renderer exists
    if (isPlaying && renderer != nullptr)
    {
        // Calculate deltaTime
        double now = juce::Time::getMillisecondCounterHiRes() * 0.001;
        double deltaTime = now - lastTime;
        lastTime = now;

        // Clamp deltaTime to avoid large jumps (e.g., after debugging)
        deltaTime = juce::jmax(0.0, juce::jmin(deltaTime, 0.1)); // Max 100ms step

        renderer->advanceAnimation(deltaTime);

        // Trigger an OpenGL repaint via the context
        openGLContext.triggerRepaint();
    }
    else if (!isPlaying)
    {
        // If paused, ensure the timer stops itself if not needed
        // Although play/pause should manage this
         if (internalTimer.isTimerRunning()) internalTimer.stopTimer(); // Call on the internal timer object
    }
}

// bool RiveComponent::loadRiveFile(const juce::File& file)
// {
//     juce::Logger::writeToLog("RiveComponent::loadRiveFile - Loading file: " + file.getFullPathName());
//
//     if (!file.existsAsFile())
//     {
//         juce::Logger::writeToLog("RiveComponent::loadRiveFile - File does not exist");
//         return false;
//     }
//
//     // Pass the file loading to the renderer
//     // Make sure the renderer is created (OpenGL context needs to be active)
//     if (openGLContext.isAttached() && renderer != nullptr)
//     {
//         renderer->loadFile(file);
//         lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Reset time
//         play(); // Start playing automatically
//         openGLContext.triggerRepaint(); // Trigger redraw
//         return true; // Assuming loadFile handles internal success/failure logging
//     }
//     else
//     {
//          juce::Logger::writeToLog("RiveComponent::loadRiveFile - OpenGL Context not ready or renderer not created.");
//          return false;
//     }
// }

// Remove loadRiveData as it duplicates logic better placed in the renderer or file loading step
/*
bool RiveComponent::loadRiveData(const void* data, size_t size)
{
    juce::Logger::writeToLog("RiveComponent::loadRiveData - Data size: " + juce::String(size) + " bytes");

    rive::ImportResult result;
    rive::Span<const uint8_t> dataSpan(static_cast<const uint8_t*>(data), size);

    // Factory is needed here! Using a member, but maybe renderer should own it?
    // For now, assuming m_riveFactory exists, which it doesn't based on header.
    // THIS NEEDS FIXING - Factory should be accessed via the renderer.
    // riveFile = rive::File::import(dataSpan, &m_riveFactory, &result);

    juce::Logger::writeToLog("RiveComponent::loadRiveData - FACTORY MISSING HERE"); return false; // TEMPORARY EXIT

    // ... rest of the logic assumes riveFile is loaded ...
}
*/

void RiveComponent::play()
{
    if (!isPlaying)
    {
        isPlaying = true;
        lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Reset time reference
        // startTimerHz(60); // Start the timer at 60Hz
        internalTimer.startTimerHz(60); // Call on the internal timer object
        juce::Logger::writeToLog("RiveComponent::play - Timer started");
    }
}

void RiveComponent::pause()
{
    if (isPlaying)
    {
        isPlaying = false;
        // stopTimer(); // Stop the timer
        internalTimer.stopTimer(); // Call on the internal timer object
        juce::Logger::writeToLog("RiveComponent::pause - Timer stopped");
    }
}

void RiveComponent::stop()
{
    if (isPlaying)
    {
        pause(); // Use pause to stop timer
    }
    
    // Reset animation state if needed (e.g., seek to beginning)
    // This requires access to the animation instance, potentially via the renderer
    // if (renderer != nullptr) renderer->resetAnimation(); // Needs implementation in renderer
    
    // Trigger one last repaint to show the stopped state
    openGLContext.triggerRepaint();
    juce::Logger::writeToLog("RiveComponent::stop - Animation stopped");
}

// Removed getAnimationCount, setAnimation as these should query the renderer
/*
int RiveComponent::getAnimationCount()
{
    // Delegate to renderer
    return (renderer != nullptr) ? renderer->getAnimationCount() : 0; // Needs implementation
}

void RiveComponent::setAnimation(int index)
{
    // Delegate to renderer
    if (renderer != nullptr)
    {
        renderer->setAnimation(index); // Needs implementation
        lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Reset time
        play(); // Assume we want to play the new animation
        juce::Logger::writeToLog("RiveComponent::setAnimation called with index: " + juce::String(index));
    }
}
*/

//==============================================================================
// OpenGL Callback Setup:
// We now rely on the attached RiveOpenGLRenderer for these callbacks.
// The RiveComponent overrides are removed.
//==============================================================================

void RiveComponent::newOpenGLContextCreated()
{
    juce::Logger::writeToLog("RiveComponent::newOpenGLContextCreated - Creating and setting RiveOpenGLRenderer");
    // Context is created, create the actual renderer instance
    if (renderer == nullptr)
        renderer = std::make_unique<RiveOpenGLRenderer>();

    // Set the created renderer as the one the context should use
    openGLContext.setRenderer(renderer.get());

    // The context will now call renderer->newOpenGLContextCreated() internally.
    // We can optionally do component-specific setup here if needed.
    play(); // Start playing animations when context is ready
}

void RiveComponent::openGLContextClosing()
{
    juce::Logger::writeToLog("RiveComponent::openGLContextClosing - Releasing RiveOpenGLRenderer");
    // Context is closing. The context will call renderer->openGLContextClosing() internally.
    // We need to release our renderer instance.
    openGLContext.setRenderer(nullptr); // Detach renderer from context
    renderer = nullptr;                 // Release ownership
}

// Remove renderOpenGL override from RiveComponent
/*
void RiveComponent::renderOpenGL()
{
    // THIS METHOD SHOULD NOT BE HERE if RiveOpenGLRenderer is attached.
    // The context calls the attached renderer's renderOpenGL directly.
    // Keeping the old logic here commented out for reference.

    // auto bounds = getLocalBounds();
    // float width = bounds.getWidth();
    // float height = bounds.getHeight();
    //
    // double currentTime = juce::Time::getMillisecondCounterHiRes() / 1000.0;
    // float deltaTime = static_cast<float>(currentTime - lastTime);
    // lastTime = currentTime;
    //
    // rive::LinearAnimationInstance* animInstance = nullptr;
    // if (currentAnimationIndex != -1 && currentAnimationIndex < animationCount) {
    //     auto anim = currentArtboard->animationAt(currentAnimationIndex);
    //     if(anim) animInstance = new rive::LinearAnimationInstance(anim); // LEAK? Needs management
    // }
    //
    // if (isPlaying && animInstance) {
    //     animInstance->advance(deltaTime);
    //     animInstance->apply(*currentArtboard, 1.0f);
    // }
    //
    // // Assuming riveRenderer here is the member of RiveComponent, which seems incorrect structure
    // // riveRenderer.render(currentArtboard, width, height, deltaTime); // This signature doesn't match RiveOpenGLRenderer::renderOpenGL()
    //
    // // delete animInstance; // LEAK if not deleted
    juce::Logger::writeToLog("RiveComponent::renderOpenGL() called - THIS SHOULD BE REMOVED");
}
*/

// Ensure the button click logic calls the correct loading function
void RiveComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        // Use unique_ptr for automatic management
        chooser = std::make_unique<juce::FileChooser> ("Select a Rive file to load...",
                                                         juce::File{},
                                                         "*.riv");

        // Replace browseForFileToOpen with the asynchronous version
        chooser->launchAsync (juce::FileBrowserComponent::openMode |
                              juce::FileBrowserComponent::canSelectFiles,
                              [this] (const juce::FileChooser& fc)
                              {
                                  auto file = fc.getResult();
                                  if (file != juce::File{})
                                  {
                                      juce::Logger::writeToLog("RiveComponent::buttonClicked - Loading file: " + file.getFullPathName());
                                      // Use the renderer to load the file
                                      if (renderer != nullptr)
                                      {
                                          renderer->loadFile(file);
                                          lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Reset time
                                          play(); // Start playing automatically
                                          openGLContext.triggerRepaint(); // Trigger redraw
                                      }
                                      else
                                      {
                                          juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::WarningIcon,
                                                                                  "Error",
                                                                                  "OpenGL Renderer not ready. Cannot load file.");
                                          juce::Logger::writeToLog("RiveComponent::buttonClicked - Renderer not available.");
                                      }
                                  }
                              });

        // Old synchronous way:
        // if (chooser.browseForFileToOpen())
        // {
        //     auto file = chooser.getResult();
        //     juce::Logger::writeToLog("RiveComponent::buttonClicked - Loading file: " + file.getFullPathName());
        //
        //     // Use the renderer to load the file
        //     if (renderer != nullptr)
        //     {
        //         renderer->loadFile(file);
        //         lastTime = juce::Time::getMillisecondCounterHiRes() * 0.001; // Reset time
        //         play(); // Start playing automatically
        //         openGLContext.triggerRepaint(); // Trigger redraw
        //     }
        //     else
        //     {
        //         juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::WarningIcon,
        //                                                 "Error",
        //                                                 "OpenGL Renderer not ready. Cannot load file.");
        //         juce::Logger::writeToLog("RiveComponent::buttonClicked - Renderer not available.");
        //     }
        // }
    }
}

// Remove loadRiveFile method from RiveComponent as logic is moved to button handler/renderer
/*
bool RiveComponent::loadRiveFile(const juce::File& file)
{
    // Redundant - handled in buttonClicked now
    return false;
}
*/

// Re-add empty implementations for pure virtual methods
void RiveComponent::initialise()
{
    juce::Logger::writeToLog("RiveComponent::initialise() (OpenGLAppComponent override) - Work delegated to RiveOpenGLRenderer via context.");
    // Actual initialization happens in RiveOpenGLRenderer::newOpenGLContextCreated
}

void RiveComponent::shutdown()
{
    juce::Logger::writeToLog("RiveComponent::shutdown() (OpenGLAppComponent override) - Work delegated to RiveOpenGLRenderer via context.");
    // Actual shutdown happens in RiveOpenGLRenderer::openGLContextClosing
}

void RiveComponent::render()
{
    // juce::Logger::writeToLog("RiveComponent::render() (OpenGLAppComponent override) - Should not be called frequently. Work delegated to RiveOpenGLRenderer::renderOpenGL via context.");
    // This is called rarely by the message thread for things like OS-level redraw requests.
    // The actual high-frequency rendering is RiveOpenGLRenderer::renderOpenGL.
    // If necessary, trigger an OpenGL repaint:
    // openGLContext.triggerRepaint();
} 