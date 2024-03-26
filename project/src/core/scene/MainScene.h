#pragma once

#include "../entity/Scene.h"

#include "../things/Player.h"

class MainScene : public Scene 
{
public:
    void init(SceneManager* sceneMgr) override;
    void update(float dt) override;
    void cleanup() override;
private:
    Player player;
};