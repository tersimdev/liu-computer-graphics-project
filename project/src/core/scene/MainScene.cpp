#include "MainScene.h"


#include "../things/Player.h"
#include "../things/LightBall.h"

void MainScene::init(Camera *camera)
{
    // Create player object
    Player *player = new Player(&mailbox, camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);


    // put more players to test
    for (int i = 1; i <= 12; ++i)
    {
        Player *temp = new Player(&mailbox, nullptr);
        temp->init();
        temp->get_drawable()->translate({i, 0, 0});
        drawables.push_back(temp->get_drawable());
        things.push_back(temp);
    }

    // todo Create maze

    // todo Create enemy spawner

    // Define lighting
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.8, 0.8, 0.6, 1);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    // add point light to test
    Light *plt = new Light;
    plt->type = LightType::POINT;
    plt->color = vec4(0, 0.8, 0.1, 1);
    plt->position = vec3(0, 0.5, -5);
    lights.push_back(plt);

    // add a drawable to point light to debug
    LightBall* lightBall = new LightBall(plt);
    lightBall->init();
    drawables.push_back(lightBall->get_drawable());
    things.push_back(lightBall);
    // example mailbox usage
    mailbox.sub(PLAYER_SAID_HELLO, lightBall); // lightBall listen to player
}

void MainScene::update(float dt)
{
    Scene::update(dt);
}
