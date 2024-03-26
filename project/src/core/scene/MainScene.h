#pragma once

#include "../entity/Scene.h"

class MainScene : public Scene 
{
    void init() override;
    void update(float dt) override;
    void cleanup() override;
};