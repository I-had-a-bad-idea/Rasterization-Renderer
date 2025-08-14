#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"
#include "RenderTarget.h"
#include "StringHelper.h"
#include "Object.h"
#include "Obj_loader.h"


class Scene{
    public:
        std::vector<Object> objects;

        virtual void Update(RenderTarget& target, float delta_time){}; // Should be overwritten

        virtual void Setup(){};
};


#endif