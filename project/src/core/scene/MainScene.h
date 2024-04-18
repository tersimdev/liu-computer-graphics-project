#pragma once

#include "Scene.h"

#include "../things/Player.h"
#include "../things/LightBall.h"
#include "../things/MazeWall.h"

class MainScene : public Scene 
{
public:
    void init(Camera* camera) override;
    void update(float dt) override;
    void on_notify(MailTopic topic, void* aux) override;
private:
    Player* player;
    LightBall* lightBall, *lightBall2;
    MazeWall* mazewall;

    double elapsed;
};