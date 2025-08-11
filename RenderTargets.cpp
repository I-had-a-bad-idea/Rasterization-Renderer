#include "Maths.h"
#include <vector>

class RenderTarget{
    public:
        int Width;
        int Height;
        float2 Size;
        std::vector<float3> color_buffer;

        RenderTarget(int width, int height){
            Width = width;
            Height = height;
            Size = float2(width, height);

        }
};