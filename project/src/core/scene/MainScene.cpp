#include "MainScene.h"

void MainScene::init(Camera* camera)
{   
    // Create player object
    Player* player = new Player(camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);
    //example mailbox usage
    mailbox.sub(PLAYER_SAID_HELLO, player); //player listen to topic
    mailbox.notify(PLAYER_SAID_HELLO); //notify player of event
    // todo Create maze

    // todo Create enemy spawner


    // Define lighting
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.8, 0.8, 0.6, 1);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    //add max point lights to test
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        Light *plt = new Light;
        plt->type = LightType::POINT;
        if (i % 2 == 0)
            plt->color = vec4(0, 0.2, 0.8, 1);
        else
            plt->color = vec4(0, 0.8, 0.1, 1);
        plt->position = vec3(-8 + i, 1, -5);
        lights.push_back(plt);
        //add a drawable to each light to debug
        Drawable* ball = dh::create_sphere(Transform(plt->position, vec3(0), vec3(0.2)), 16, 16);
        ball->setMaterial(dh::create_material(SIMPLE, plt->color, {0,0,0,0}));
        drawables.push_back(ball);
    }
}

void MainScene::update(float dt)
{
    Scene::update(dt);
}
