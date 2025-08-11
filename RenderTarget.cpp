#include "RenderTarget.h"

RenderTarget::RenderTarget(int width, int height){
    Width = width;
    Height = height;
    Size = float2(width, height);
}