#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "Vector.h"
#include <vector>

class RenderTarget {
public:
    int Width;
    int Height;
    float2 Size;
    std::vector<float3> color_buffer;
    std::vector<float> depth_buffer;

    RenderTarget(int width, int height);
};

#endif