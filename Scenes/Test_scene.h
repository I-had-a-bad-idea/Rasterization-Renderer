#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Scene.h"

class TestScene : public Scene{
    public:
        void Update(RenderTarget& target, float delta_time);
        void Setup();
};




#endif