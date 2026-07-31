#include "RenderTarget.h"

RenderTarget::RenderTarget(int width, int height){
    Width = width;
    Height = height;
    Size = float2(width, height);
    color_buffer.resize(width * height);
    // Initialize depth buffer with maximum depth values
    depth_buffer.resize(width * height, 1000.0f);
}