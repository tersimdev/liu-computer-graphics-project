#pragma once

#include "Scene.h"

class MainScene : public Scene 
{
public:
    void init(Camera* camera) override;
    void update(float dt) override;
};