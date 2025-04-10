#pragma once

#include <cstdint>  // For uint32_t, etc.
#include <string>   // For std::string

#include <JuceHeader.h>
#include <juce_opengl/juce_opengl.h>
#include <memory>

// Forward declarations for Rive types we only use as pointers/references
namespace rive {
    class Artboard;
    class Renderer;
    class RenderBuffer;
    class Factory;
}

// Include necessary OpenGL-specific headers if needed
#ifdef _WIN32
#include <gl/GL.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Include actual Rive headers now
#include <rive/file.hpp>
#include <rive/artboard.hpp>
#include <rive/animation/linear_animation_instance.hpp>
#include <rive/renderer.hpp>
#include <rive/factory.hpp>
#include <rive/math/math_types.hpp>

// If using the low-level Rive renderer directly (less likely with juce_rive)
// #include <rive/renderer.hpp> 

//==============================================================================
class RiveOpenGLRenderer : public juce::OpenGLRenderer
{
public:
    RiveOpenGLRenderer();
    ~RiveOpenGLRenderer() override;

    // Required juce::OpenGLRenderer overrides
    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    // Your existing methods
    void initialize();
    void render(const juce::AffineTransform& transform, float width, float height, double deltaTime);
    void cleanup();
    void loadFile(const juce::File& file);
    void advanceAnimation(double deltaTime);

private:
    std::unique_ptr<rive::Renderer> renderer;
    std::unique_ptr<rive::RenderBuffer> renderBuffer;
    std::unique_ptr<rive::Factory> factory;
    
    // OpenGL state
    int viewportWidth = 0;
    int viewportHeight = 0;
    
    void setupViewport(int width, int height);
    void setupProjectionMatrix(float width, float height);

    // Helper function for calculating view transform
    rive::Mat2D calculateTransform(float viewWidth,
                                   float viewHeight,
                                   const rive::ArtboardInstance& artboard,
                                   rive::Fit fit,
                                   rive::Alignment alignment);

    std::unique_ptr<rive::File> riveFile;
    std::unique_ptr<rive::ArtboardInstance> artboardInstance;
    std::unique_ptr<rive::LinearAnimationInstance> animationInstance;
    double animationTime = 0.0;
    juce::OpenGLContext* context = nullptr; // Store context if needed

    // Simple GLSL shaders (replace with actual shaders if needed)
    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram;
    const char* vertexShader;
    const char* fragmentShader;

    // You'll need methods/members to handle Rive rendering specifics
    // e.g., setting up Rive's renderer backend if not using juce::RiveComponent
    // std::unique_ptr<rive::Renderer> riveRenderer; 
}; 