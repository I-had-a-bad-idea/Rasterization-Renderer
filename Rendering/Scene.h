#ifndef SCENE_H
#define SCEN_H

#include <vector>
#include "Object.h"
#include "RenderTarget.h"

class Scene{
    public: 
        std::vector<Object> objects;

        void Update(RenderTarget target, float delta_time); // Should be overwritten
};


#endif