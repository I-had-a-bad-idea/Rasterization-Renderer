#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "Object_mesh.h"
#include <memory>

class TextureShader : public ObjectShader{
    public:
        std::shared_ptr<MeshTexture> Shader_texture;
        TextureShader(std::shared_ptr<MeshTexture> shader_texture);
        
        float3 PixelColor(float2 tex_coords) const override;
};


#endif