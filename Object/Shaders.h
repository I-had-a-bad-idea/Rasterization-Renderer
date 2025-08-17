#ifndef SHADERS_H
#define SHADERS_H

#include "Object_mesh.h"
#include <memory>
#include "Maths.h"

class TextureShader : public ObjectShader{
    public:
        std::shared_ptr<MeshTexture> Shader_texture;
        TextureShader(std::shared_ptr<MeshTexture> shader_texture);
        
        float3 PixelColor(float2 tex_coords, float3 normal) const override;
};

class LitTextureShader : public ObjectShader{
    public:
        std::shared_ptr<MeshTexture> Shader_texture;
        float3 Direction_to_light;
        float Light_intensity = 1.0;

        LitTextureShader(std::shared_ptr<MeshTexture> shader_texture, float3 direction_to_light);
        
        float3 PixelColor(float2 tex_coords, float3 normal) const override;

};


#endif