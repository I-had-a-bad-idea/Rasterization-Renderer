#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"
#include "RenderTarget.h"
#include "StringHelper.h"
#include "Object.h"
#include "Obj_loader.h"
#include "Camera.h"
#include <SDL2/SDL.h>

class Scene{
    public:
        std::vector<Object> objects;
        Cam camera;

        virtual void Update(RenderTarget& target, float delta_time){}; // Should be overwritten

        virtual void Setup(){};
};


#endif