#pragma once

#include "renderer.hpp"

namespace rive {

class Artboard {
public:
    Artboard() = default;
    virtual ~Artboard() = default;
    
    // Method needed for our renderer
    void draw(Renderer* renderer) {}
    
    // Animation related methods
    int animationCount() const { return 0; }
    void* animationAt(int index) { return nullptr; }
};

} // namespace rive 