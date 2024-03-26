#include "MainScene.h"

void MainScene::init(Camera* camera)
{   
    // Create player object
    Player* player = new Player(camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);

    // todo Create maze

    // todo Create enemy spawner


    // Define lighting
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.8, 0.8, 0.6, 1);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);
}

void MainScene::update(float dt)
{
    Scene::update(dt);
}
