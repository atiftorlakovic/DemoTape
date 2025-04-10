#pragma once

#include "math/mat2d.hpp"
#include "render_path.hpp"
#include "render_paint.hpp"
#include "render_image.hpp"
#include "render_buffer.hpp"
#include "shapes/paint/blend_mode.hpp"

namespace rive {

// Base renderer abstract class
class Renderer {
public:
    Renderer() = default;
    virtual ~Renderer() = default;
    
    // Required virtual methods
    virtual void save() = 0;
    virtual void restore() = 0;
    virtual void transform(const Mat2D& transform) = 0;
    virtual void drawPath(RenderPath* path, RenderPaint* paint) = 0;
    virtual void clipPath(RenderPath* path) = 0;
    virtual void drawImage(const RenderImage* image, BlendMode blendMode, float opacity) = 0;
    
    // For image mesh drawing
    virtual void drawImageMesh(
        const RenderImage* image,
        rcp<RenderBuffer> vertices,
        rcp<RenderBuffer> uvCoords,
        rcp<RenderBuffer> indices,
        uint32_t vertexCount,
        uint32_t indexCount,
        BlendMode blendMode,
        float opacity) = 0;
};

} // namespace rive 