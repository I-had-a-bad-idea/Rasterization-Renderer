#ifndef SHADERS_H
#define SHADERS_H

#include "Object_mesh.h"
#include <memory>
#include "Maths.h"

enum class ShaderType {
    Texture,
    LitTexture
};

class ObjectShader {
public:
    ShaderType type;
    virtual ~ObjectShader() = default;
};

class TextureShader : public ObjectShader {
public:
    std::shared_ptr<MeshTexture> Shader_texture;
    TextureShader(std::shared_ptr<MeshTexture> tex) {
        Shader_texture = std::move(tex);
        type = ShaderType::Texture;
    }
};

class LitTextureShader : public ObjectShader {
public:
    std::shared_ptr<MeshTexture> Shader_texture;
    float3 Direction_to_light;
    LitTextureShader(std::shared_ptr<MeshTexture> tex, float3 dir) {
        Shader_texture = std::move(tex);
        Direction_to_light = dir;
        type = ShaderType::LitTexture;
    }
};

#endif