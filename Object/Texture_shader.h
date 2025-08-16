#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "Object_mesh.h"

class TextureShader : public ObjectShader{
    public:
        MeshTexture Shader_texture;
        TextureShader(MeshTexture Shader_texture);
        
        float3 PixelColor(float2 tex_coords) const override;
};


#endif