#include "Obj_loader.h"
#include <sstream>
#include <iostream>
#include <array>

Object ObjLoader::load_object(std::string path, float3 position, float3 rotation, std::string name){
    std::string obj_path = std::filesystem::current_path().string() + path;
    std::string obj_string = StringHelper::readFileToString(obj_path);
    auto mesh_data = load_obj_file(obj_string);

    std::vector<float3> object_points = std::get<0>(mesh_data);
    std::vector<float3> object_normals = std::get<1>(mesh_data);
    std::vector<float2> texture_coords = std::get<2>(mesh_data);

    if (object_points.empty()) {
       std::cerr << "Failed to load model or model is empty!" << std::endl;
       throw std::runtime_error("Failed to load model: " + path);
    }

    // Create random colors
    std::vector<float3> triangle_colors(object_points.size() / 3);
    for (size_t i = 0; i < triangle_colors.size(); i++) {
        triangle_colors[i] = Math::random_color();
    }

    // Load texture + shader
    auto texture = std::make_shared<MeshTexture>(
        MeshTexture::CreateFromBytes(ImageLoader::png_file_to_bytes("Test_Texture.png"))
    );
    auto shader = std::make_shared<TextureShader>(texture);

    ObjectMesh mesh(object_points, object_normals, texture_coords);
    return Object(mesh, shader, name, triangle_colors, position, rotation);
}


// Proper OBJ parser
std::tuple<std::vector<float3>, std::vector<float3>, std::vector<float2>> 
ObjLoader::load_obj_file(std::string objString) {

    std::vector<float3> positions;
    std::vector<float3> normals_all;
    std::vector<float2> texcoords_all;

    std::vector<float3> out_vertices;
    std::vector<float3> out_normals;
    std::vector<float2> out_texcoords;

    std::istringstream stream(objString);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.rfind("v ", 0) == 0) { // vertex position
            auto parts = StringHelper::split(line.substr(2), ' ');
            if (parts.size() < 3) continue;
            positions.emplace_back(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
        }
        else if (line.rfind("vt", 0) == 0) {
            auto parts = StringHelper::split(line.substr(3), ' ');
            if (parts.size() < 2) continue;
            float u = std::stof(parts[0]);
            float v = std::stof(parts[1]);
            texcoords_all.emplace_back(u, 1.0f - v); // flip V
        }
        else if (line.rfind("vn", 0) == 0) {
            auto parts = StringHelper::split(line.substr(3), ' ');
            if (parts.size() < 3) continue;
            normals_all.emplace_back(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
        }
        else if (line.rfind("f ", 0) == 0) {
            auto faceIndexGroups = StringHelper::split(line.substr(2), ' ');

            // Triangulate (fan method)
            for (int i = 1; i + 1 < (int)faceIndexGroups.size(); i++) {
                std::array<std::string,3> tri = { faceIndexGroups[0], faceIndexGroups[i], faceIndexGroups[i+1] };
                for (auto& vertStr : tri) {
                    auto idx = StringHelper::split(vertStr, '/');
                    int vi = (idx.size() > 0 && !idx[0].empty()) ? std::stoi(idx[0]) - 1 : -1;
                    int ti = (idx.size() > 1 && !idx[1].empty()) ? std::stoi(idx[1]) - 1 : -1;
                    int ni = (idx.size() > 2 && !idx[2].empty()) ? std::stoi(idx[2]) - 1 : -1;

                    if (vi >= 0) out_vertices.push_back(positions[vi]);
                    if (ti >= 0) out_texcoords.push_back(texcoords_all[ti]);
                    else out_texcoords.emplace_back(0,0);
                    if (ni >= 0) out_normals.push_back(normals_all[ni]);
                    else out_normals.emplace_back(0,0,1);
                }
            }
        }
    }

    return std::make_tuple(out_vertices, out_normals, out_texcoords);
}
