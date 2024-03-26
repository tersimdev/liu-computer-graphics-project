#pragma once

#include "Scene.h"

#include "../things/Player.h"

class MainScene : public Scene 
{
public:
    void init(Camera* camera) override;
    void update(float dt) override;
};