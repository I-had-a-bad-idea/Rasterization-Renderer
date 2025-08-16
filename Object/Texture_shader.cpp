
#include "Texture_shader.h"


TextureShader::TextureShader(MeshTexture shader_texture){
    Shader_texture = shader_texture;
}

float3 TextureShader::PixelColor(float2 texture_coords) const {
    return Shader_texture.Sample(texture_coords.x, texture_coords.y);
}