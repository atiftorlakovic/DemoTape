#pragma once

namespace rive {

class Mat2D {
public:
    // Basic constructor
    Mat2D() {
        // Identity matrix
        values[0] = 1.0f;
        values[1] = 0.0f;
        values[2] = 0.0f;
        values[3] = 1.0f;
        values[4] = 0.0f;
        values[5] = 0.0f;
    }
    
    // Values stored in column-major order
    float values[6];
};

} // namespace rive 