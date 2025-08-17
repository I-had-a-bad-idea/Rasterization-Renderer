
#include "Shaders.h"


TextureShader::TextureShader(std::shared_ptr<MeshTexture> shader_texture){
    Shader_texture = std::move(shader_texture);
}

float3 TextureShader::PixelColor(float2 texture_coords, float3 normal) const {
    return Shader_texture->Sample(texture_coords.x, texture_coords.y);
}

LitTextureShader::LitTextureShader(std::shared_ptr<MeshTexture> shader_texture, float3 direction_to_light){
    Shader_texture = std::move(shader_texture);
    Direction_to_light = direction_to_light;
}

float3 LitTextureShader::PixelColor(float2 texture_coords, float3 normal) const{
    float light_intensity = (Math::dot(normal, Direction_to_light) + 1) * 0.5;
    light_intensity = Math::lerp(0.4, 1, light_intensity);
    return Shader_texture->Sample(texture_coords.x, texture_coords.y) * light_intensity;
}

