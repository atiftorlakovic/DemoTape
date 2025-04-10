#include "juce_config.h"  // Include JUCE config first
#include <JuceHeader.h>
#include "RiveOpenGLRenderer.h"

// Use our stub Rive headers directly with correct paths
#include <rive/file.hpp>        // Include file.hpp, which includes artboard.hpp
#include <rive/math/mat2d.hpp>
#include <rive/shapes/paint/blend_mode.hpp>
#include <rive/refcnt.hpp>
#include <rive/renderer.hpp> // Renderer includes render_buffer.hpp usually
#include <rive/renderer/rive_render_image.hpp>
#include <rive/renderer/rive_render_paint.hpp>
#include <rive/render_path.hpp>
#include <rive/shapes/paint/stroke_cap.hpp>
#include <rive/shapes/paint/stroke_join.hpp>
#include <rive/shapes/paint/color.hpp>
#include <rive/renderer_paint_style.hpp>
#include <rive/factory.hpp> // Include Rive factory header
#include <rive/math/math_types.hpp> // For Alignment & Fit

// Minimal Rive Factory Implementation (Stub)
class StubFactory : public rive::Factory
{
public:
    rive::rcp<rive::RenderBuffer> makeRenderBuffer(rive::RenderBufferType type,
                                                   rive::RenderBufferFlags flags,
                                                   size_t sizeInBytes) override
    {
        // Return null or implement based on actual renderer needs
        return nullptr;
    }

    rive::rcp<rive::RenderShader> makeRenderShader(rive::rcp<rive::RenderBuffer> buffer)
    {
        // Return null or implement
        return nullptr;
    }

    rive::rcp<rive::RenderPath> makeRenderPath(rive::rcp<rive::RenderBuffer> points,
                                               rive::rcp<rive::RenderBuffer> verbs,
                                               rive::FillRule fillRule) override
    {
        // Return null or implement
        return nullptr;
    }

    rive::rcp<rive::RenderPaint> makeRenderPaint() override
    {
        // Return null or implement
        return nullptr;
    }

    rive::rcp<rive::RenderImage> decodeImage(rive::Span<const uint8_t> data) override
    {
        // Return null as we don't handle embedded images yet
        return nullptr;
    }
};

// Simple renderer implementation
class SimpleRenderer : public rive::Renderer
{
public:
    // Ensure signatures exactly match base Renderer
    // (May require adding const or checking Rive headers for exact signature)
    void save() override {}
    void restore() override {}
    void transform(const rive::Mat2D& transform) override {}
    
    // Implement drawPath with corrected method calls
    void drawPath(rive::RenderPath* path, rive::RenderPaint* paint) override 
    {
        // Using the correct API methods for RenderPaint
        paint->style(rive::RenderPaintStyle::stroke);
        paint->thickness(10.0f);
        paint->strokeJoin(rive::StrokeJoin::miter);
        paint->strokeCap(rive::StrokeCap::butt);
        paint->color(0xFF0000FF);
        
        // Note: Actual rendering would require converting the path to OpenGL
        // primitives and setting up shaders, etc. This is just a stub.
    }
    
    void clipPath(rive::RenderPath* path) override {}
    void drawImage(const rive::RenderImage* image, rive::BlendMode blendMode, float opacity) override {}
    
    // Define drawImageMesh inline
    void drawImageMesh(const rive::RenderImage* image,
                      rive::rcp<rive::RenderBuffer> vertices,
                      rive::rcp<rive::RenderBuffer> uvCoords,
                      rive::rcp<rive::RenderBuffer> indices,
                      uint32_t vertexCount,
                      uint32_t indexCount,
                      rive::BlendMode blendMode,
                      float opacity) override {}
};

// Simple render buffer implementation - Keep commented out for now
/*
class SimpleRenderBuffer : public rive::RenderBuffer
{
public:
    SimpleRenderBuffer(rive::RenderBufferType type, rive::RenderBufferFlags flags, size_t sizeInBytes) :
        rive::RenderBuffer(type, flags, sizeInBytes)
    {}

    void* onMap() override { return nullptr; } 
    void onUnmap() override { }
};
*/

// Constructor needs definition (even if empty)
// Note: Constructor might need OpenGLContext reference depending on full implementation
RiveOpenGLRenderer::RiveOpenGLRenderer()
{
    // Constructor implementation
    factory = std::make_unique<StubFactory>(); // Instantiate the factory
}

RiveOpenGLRenderer::~RiveOpenGLRenderer()
{
    // Destructor implementation - ensure cleanup happens
    // cleanup(); // Might call this from openGLContextClosing instead
}

//==============================================================================
// juce::OpenGLRenderer overrides
//==============================================================================

void RiveOpenGLRenderer::newOpenGLContextCreated()
{
    juce::Logger::writeToLog("RiveOpenGLRenderer: newOpenGLContextCreated() called.");
    // Context is available, perform OpenGL initialisation that requires a context
    initialize(); // Call your existing GL setup

    // You might want to store the context if needed later
    context = &juce::OpenGLContext::getCurrentContext();
}

void RiveOpenGLRenderer::renderOpenGL()
{
    // Main OpenGL render callback from JUCE
    // Note: This is DIFFERENT from your existing render() method.
    // We need to adapt how Rive drawing is triggered.

    // 1. Check if context and Rive resources are ready
    if (!artboardInstance || !context)
        return;

    // 2. Setup OpenGL state (JUCE helpers can manage some of this)
    juce::OpenGLHelpers::clear(juce::Colours::transparentBlack); // Example clear
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 3. Get viewport size from the context
    float w = (float)context->getRenderingScale() * context->getFrameBufferWidth();
    float h = (float)context->getRenderingScale() * context->getFrameBufferHeight();
    // setupViewport(w, h); // Call your viewport setup if needed
    
    // 4. Calculate necessary transforms (if not handled internally by Rive renderer)
    juce::AffineTransform transform = juce::AffineTransform::identity; // Calculate actual transform if needed
    
    // 5. Advance animation (this should ideally happen based on time, maybe in RiveComponent::timerCallback)
    // advanceAnimation(); // Moved to timer

    // 6. Call your actual Rive drawing logic
    // This needs the Rive renderer instance setup correctly
    // Ensure your SimpleRenderer (or equivalent) is initialized and used here.
    // If using the Rive low-level renderer, you'd call artboardInstance->draw(riveRenderer.get());
    // Example placeholder:
    if (riveRenderer) // Assuming you have a member std::unique_ptr<rive::Renderer> riveRenderer;
    {
        riveRenderer->save();
        // Apply alignment/transform
        riveRenderer->transform(calculateTransform(w,
                                                   h,
                                                   *artboardInstance,
                                                   rive::Fit::contain, // Example fit
                                                   rive::Alignment::center // Example alignment
                                                   ));
        artboardInstance->draw(riveRenderer.get());
        riveRenderer->restore();
    }
}

void RiveOpenGLRenderer::openGLContextClosing()
{
    juce::Logger::writeToLog("RiveOpenGLRenderer: openGLContextClosing() called.");
    // Context is about to close, release OpenGL resources
    cleanup(); // Call your existing GL cleanup
    shaderProgram = nullptr; // Release shaders
    riveRenderer = nullptr;  // Release Rive renderer
    context = nullptr;
}

//==============================================================================
// Your existing methods 
//==============================================================================

void RiveOpenGLRenderer::initialize()
{
    juce::Logger::writeToLog("RiveOpenGLRenderer: initialize() called.");
    // Your existing OpenGL initialization code
    // Setup shaders, Rive renderer backend etc.
    
    // Example shader setup (replace with your actual shaders)
    vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main()
        {
           gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";
    fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange placeholder
        }
    )";

    shaderProgram = std::make_unique<juce::OpenGLShaderProgram>(juce::OpenGLContext::getCurrentContext());
    if (!shaderProgram->addShaders(vertexShader, fragmentShader))
    {
        juce::Logger::writeToLog("Failed to compile GLSL shaders!");
    }
    
    // Initialize Rive's renderer if you are managing it manually
    // auto* backendContext = GetRiveBackendContext(); // Platform specific
    // riveRenderer = rive::Renderer::Make(backendContext);
    
    // Initialize our simple renderer stub (replace with actual GL backend later)
    riveRenderer = std::make_unique<SimpleRenderer>(); 
    juce::Logger::writeToLog("Rive SimpleRenderer initialized.");
}

// ... Rest of your existing methods like render(...), cleanup(), loadFile(), advanceAnimation() ...
// Note: The existing render() method might need to be removed or adapted, as renderOpenGL() is now the main callback.

void RiveOpenGLRenderer::render(const juce::AffineTransform& transform, float width, float height, double deltaTime)
{
    // This method might be redundant now with renderOpenGL()
    // Decide if you need this specific signature or if all drawing happens in renderOpenGL()
    juce::Logger::writeToLog("RiveOpenGLRenderer: render(...) called - (Consider removing or adapting)");
}

void RiveOpenGLRenderer::cleanup()
{
    juce::Logger::writeToLog("RiveOpenGLRenderer: cleanup() called.");
    // Your existing cleanup code
    shaderProgram = nullptr;
    riveFile = nullptr;
    artboardInstance = nullptr;
    animationInstance = nullptr;
    // riveRenderer = nullptr;
}

// ... implementations for loadFile, advanceAnimation etc. ...

void RiveOpenGLRenderer::loadFile(const juce::File& file)
{
    juce::Logger::writeToLog("RiveOpenGLRenderer: Loading file: " + file.getFullPathName());
    juce::MemoryBlock fileData;
    if (file.loadFileAsData(fileData))
    {
        // Ensure rive::Factory is implemented or provided
        // MinimalRiveFactory factory; // Use your factory implementation
        rive::ImportResult result;
        auto bytes = rive::make_span(fileData.getData(), fileData.getSize());
        // riveFile = rive::File::import(bytes, &factory, &result); // Pass factory instance
        riveFile = rive::File::import(bytes, factory.get(), &result); // Use the member factory

        if (riveFile)
        {
            artboardInstance = riveFile->artboardDefault();
            if (artboardInstance)
            {
                animationInstance = artboardInstance->animationDefault(); 
                animationTime = 0.0;
                 juce::Logger::writeToLog("Rive file loaded successfully.");
            }
            else
            {
                juce::Logger::writeToLog("Failed to instantiate default artboard.");
                riveFile = nullptr; 
            }
        }
        else
        {
             juce::Logger::writeToLog("Failed to import Rive file. Result: " + juce::String((int)result));
        }
    }
    else
    {
        juce::Logger::writeToLog("Failed to load file data.");
    }
}

void RiveOpenGLRenderer::advanceAnimation(double deltaTime)
{
    if (animationInstance && artboardInstance)
    {
        // IMPORTANT: Use actual delta time from a timer!
        // Using a fixed step here for demonstration.
        // double deltaTime = 1.0 / 60.0; // Example: Simulate 60 FPS - REMOVED

        // Ensure deltaTime is non-negative
        if (deltaTime < 0) deltaTime = 0;

        animationTime += deltaTime;
        // Loop the animation
        if (animationTime >= animationInstance->durationSeconds())
        {
            animationTime = fmod(animationTime, animationInstance->durationSeconds());
        }

        // Apply time to the animation instance
        animationInstance->time(animationTime);

        // Apply the animation changes to the artboard instance
        animationInstance->apply(*artboardInstance);

        // Advance the artboard instance itself (important!)
        artboardInstance->advance(deltaTime);

        // juce::Logger::writeToLog("Advanced animation: " + juce::String(animationTime)); // Optional logging
    }
}

//==============================================================================
// Internal Helper Methods
//==============================================================================

void RiveOpenGLRenderer::setupViewport(int width, int height)
{
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, viewportWidth, viewportHeight);
    juce::Logger::writeToLog("RiveOpenGLRenderer: setupViewport(" + juce::String(width) + ", " + juce::String(height) + ")");
}

void RiveOpenGLRenderer::setupProjectionMatrix(float width, float height)
{
    // This setup assumes a simple orthographic projection.
    // Modern OpenGL with shaders often incorporates this into the vertex shader's matrix uniforms.
    // The Rive renderer might also handle its own transformations.
    // This is a placeholder based on traditional fixed-function pipeline style.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f); // Top-left origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     juce::Logger::writeToLog("RiveOpenGLRenderer: setupProjectionMatrix(" + juce::String(width) + ", " + juce::String(height) + ")");
}

// Helper function to calculate the transformation matrix
rive::Mat2D RiveOpenGLRenderer::calculateTransform(float viewWidth,
                                                  float viewHeight,
                                                  const rive::ArtboardInstance& artboard,
                                                  rive::Fit fit,
                                                  rive::Alignment alignment)
{
    if (!context || viewWidth <= 0 || viewHeight <= 0)
        return rive::Mat2D(); // Return identity if no context or invalid size

    // Calculate the transform needed to fit the artboard into the view bounds
    // based on the specified fit and alignment.
    return rive::computeAlignment(fit,
                                  alignment,
                                  rive::AABB(0, 0, viewWidth, viewHeight), // Frame bounds
                                  artboard.bounds()); // Artboard bounds
}