#pragma once

namespace rive {

// Forward declarations
enum class StrokeJoin;
enum class StrokeCap;
enum class RenderPaintStyle;

class RenderPaint {
public:
    // Constructor/destructor
    RenderPaint() = default;
    virtual ~RenderPaint() = default;
    
    // Methods needed for our implementation
    virtual void style(RenderPaintStyle style) {}
    virtual void thickness(float thickness) {}
    virtual void strokeJoin(StrokeJoin join) {}
    virtual void strokeCap(StrokeCap cap) {}
    virtual void color(unsigned int color) {}
};

} // namespace rive 