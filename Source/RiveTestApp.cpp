#include <JuceHeader.h>
#include "RiveComponent.h"

class RiveTestWindow : public juce::DocumentWindow
{
public:
    RiveTestWindow() : juce::DocumentWindow("Rive Test", juce::Colours::darkgrey, juce::DocumentWindow::allButtons)
    {
        // Create main content component
        riveComponent = std::make_unique<RiveComponent>();
        setContentOwned(riveComponent.get(), true);
        
        // Setup window properties
        centreWithSize(800, 600);
        setVisible(true);
        setResizable(true, true);
        
        // Load the Rive file
        juce::Logger::writeToLog("Starting Rive Test Application");
        loadRiveFile();
    }
    
    ~RiveTestWindow() override
    {
        // Clean up
        riveComponent = nullptr;
    }
    
    void closeButtonPressed() override
    {
        // Quit the application when the window is closed
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
private:
    std::unique_ptr<RiveComponent> riveComponent;
    
    void loadRiveFile()
    {
        // Use the helper method to load the demo Rive file
        juce::Logger::writeToLog("RiveTestApp: Loading demo Rive file");
        bool loaded = riveComponent->loadDemoRiveFile();
        
        if (loaded)
        {
            juce::Logger::writeToLog("RiveTestApp: Successfully loaded demo Rive file");
            riveComponent->play(); // Auto-play the animation
        }
        else
        {
            juce::Logger::writeToLog("RiveTestApp: Failed to load demo Rive file");
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                 "Error",
                                                 "Failed to load demo Rive file. Check the log for details.");
        }
    }
};

//==============================================================================
class RiveTestApplication : public juce::JUCEApplication
{
public:
    RiveTestApplication() {}
    
    const juce::String getApplicationName() override { return "Rive Test"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String&) override
    {
        // Create the main window
        mainWindow = std::make_unique<RiveTestWindow>();
    }
    
    void shutdown() override
    {
        // Clean up
        mainWindow = nullptr;
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
private:
    std::unique_ptr<RiveTestWindow> mainWindow;
};

//==============================================================================
// This macro creates the application's main() function
START_JUCE_APPLICATION(RiveTestApplication) 