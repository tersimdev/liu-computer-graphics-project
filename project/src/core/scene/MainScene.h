#pragma once

#include "../entity/Scene.h"

class MainScene : public Scene 
{
    void init(SceneManager* sceneMgr) override;
    void update(float dt) override;
    void cleanup() override;
};