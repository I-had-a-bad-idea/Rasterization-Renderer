
#include "Texture_shader.h"


TextureShader::TextureShader(std::shared_ptr<MeshTexture> shader_texture){
    Shader_texture = std::move(shader_texture);
}

float3 TextureShader::PixelColor(float2 texture_coords) const {
    return Shader_texture->Sample(texture_coords.x, texture_coords.y);
}