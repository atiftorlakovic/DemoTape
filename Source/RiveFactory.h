#pragma once

#include <JuceHeader.h>
#include <memory>

// Core Rive includes
#include <rive/factory.hpp>
#include <rive/refcnt.hpp>
#include <rive/math/mat2d.hpp>
#include <rive/math/raw_path.hpp>
#include <rive/shapes/paint/blend_mode.hpp>
#include <rive/shapes/paint/stroke_cap.hpp>
#include <rive/shapes/paint/stroke_join.hpp>
#include <rive/renderer.hpp>

// Forward declarations not needed since we have full includes above

// --- Minimal Stub Implementations Needed by Factory ---

class StubRenderPath : public rive::RenderPath {
public:
    // Provide minimal implementations for pure virtual methods
    void reset() override {}
    void addRenderPath(rive::RenderPath* /*path*/, const rive::Mat2D& /*transform*/) override {}
    void fillRule(rive::FillRule /*value*/) override {}
    void moveTo(float /*x*/, float /*y*/) override {}
    void lineTo(float /*x*/, float /*y*/) override {}
    void cubicTo(float /*ox*/, float /*oy*/, float /*ix*/, float /*iy*/, float /*x*/, float /*y*/) override {}
    void close() override {}
};

class StubRenderPaint : public rive::RenderPaint {
public:
    // Provide minimal implementations for pure virtual methods
    void style(rive::RenderPaintStyle /*style*/) override {}
    void color(unsigned int /*value*/) override {}
    void thickness(float /*value*/) override {}
    void join(rive::StrokeJoin /*value*/) override {}
    void cap(rive::StrokeCap /*value*/) override {}
    void blendMode(rive::BlendMode /*value*/) override {}
    void shader(rive::rcp<rive::RenderShader> /*shader*/) override {}
    void invalidateStroke() override {}
};

// --- Minimal Factory Implementation ---

class MinimalRiveFactory : public rive::Factory
{
public:
    // Creates low-level buffers (like vertex/index buffers). Return nullptr if your
    // renderer handles buffer creation separately *after* loading.
    rive::rcp<rive::RenderBuffer> makeRenderBuffer(rive::RenderBufferType /*type*/,
                                               rive::RenderBufferFlags /*flags*/,
                                               size_t /*sizeInBytes*/) override
    {
        return nullptr;
    }

    // Creates shader objects. Return nullptr if not needed during file load.
    rive::rcp<rive::RenderShader> makeLinearGradient(float /*sx*/, float /*sy*/, float /*ex*/, float /*ey*/,
                                               const rive::ColorInt /*colors*/[], const float /*stops*/[],
                                               size_t /*count*/) override
    {
        return nullptr;
    }

    rive::rcp<rive::RenderShader> makeRadialGradient(float /*cx*/, float /*cy*/, float /*radius*/,
                                               const rive::ColorInt /*colors*/[], const float /*stops*/[],
                                               size_t /*count*/) override
    {
        return nullptr;
    }

    // Creates path objects. Crucial for loading shapes. Return a stub implementation.
    rive::rcp<rive::RenderPath> makeRenderPath(rive::RawPath& /*rawPath*/, rive::FillRule /*fillRule*/) override
    {
        // Note: No extra state/members in StubRenderPath compared to the previous version
        return rive::make_rcp<StubRenderPath>();
    }

    // Deprecated but might be called.
    rive::rcp<rive::RenderPath> makeEmptyRenderPath() override
    {
        return rive::make_rcp<StubRenderPath>();
    }

    // Creates paint objects. Crucial for loading fills/strokes. Return a stub.
    rive::rcp<rive::RenderPaint> makeRenderPaint() override
    {
        // Note: No extra state/members in StubRenderPaint compared to the previous version
        return rive::make_rcp<StubRenderPaint>();
    }

    // Creates image objects. Return nullptr if you handle image loading/decoding later.
    rive::rcp<rive::RenderImage> decodeImage(rive::Span<const uint8_t> /*bytes*/) override
    {
        return nullptr;
    }

    // Note: decodeFont and decodeAudio have default implementations in rive::Factory base
}; 