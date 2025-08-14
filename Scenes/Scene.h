#ifndef SCENE_H
#define SCEN_H

#include <vector>
#include "Object.h"
#include "RenderTarget.h"
#include "StringHelper.h"
#include <file

Object load_object(std::string path, float3 position, float3 rotatation){
    // Load model data
    std::string obj_path = std::filesystem::current_path().string() + path;
    std::string obj_string = StringHelper::readFileToString(obj_path);
    std::vector<float3> object_points = ObjLoader::load_obj_file(obj_string);

class Scene{
    public: 
        std::vector<Object> objects;

        virtual void Update(RenderTarget target, float delta_time) = 0; // Should be overwritten

        virtual void Setup() = 0;
};


#endif