#include "MainScene.h"

void MainScene::init()
{   
    // Creating player object
    Drawable *d = dh::create_from_model("bunnyplus.obj", Transform({0,0,-5}, {0,-45,0}, {0.5, 0.5, 0.5}));
    Material *m = dh::create_material("test", ShaderProg::LIT, {0.7, 0.2, 0.2}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "conc");
    d->setMaterial(m);
    drawables.push_back(d);
    Player* player = new Player;
    player->init();
    player->attach_drawable(d);
    things.push_back(player);

    //todo create maze
    // todo create enemy spawner

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
