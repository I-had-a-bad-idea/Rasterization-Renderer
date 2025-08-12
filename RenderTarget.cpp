#include "RenderTarget.h"

RenderTarget::RenderTarget(int width, int height){
    Width = width;
    Height = height;
    Size = float2(width, height);
    color_buffer.resize(width * height);
    depth_buffer.resize(width * height, std::numeric_limits<float>::infinity());
}