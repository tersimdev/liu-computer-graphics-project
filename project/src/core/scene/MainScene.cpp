#include "MainScene.h"

#include "../system/SceneManager.h"
#include "../system/DrawableHelper.h"

void MainScene::init(SceneManager *sceneMgr)
{
    GraphicsManager* graphicsMgr = sceneMgr->get_graphics_mgr();
    TextureManager* textureMgr = sceneMgr->get_texture_mgr();
    
    Drawable *d = DrawableHelper::create_from_model("bunnyplus.obj", Transform({0,0,-5}, {0,-45,0}, {0.5, 0.5, 0.5}));
    Material *m = textureMgr->create_material("test", ShaderProg::LIT, {0.7, 0.2, 0.2}, {0.8, 0.8, 0.8, 16});
    textureMgr->attach_texture_to_material(m, 0, textureMgr->get_texture("conc"));
    d->setMaterial(m);
    graphicsMgr->add_obj(d);

    Light *dlt = new Light;
    dlt->color = vec4(0.8, 0.8, 0.6, 1);
    dlt->position = vec3(-1, -1, -0.5);
    graphicsMgr->set_dir_light(dlt);

    // create Things here like Player()
    // add Player to ThingManager!
    // Initialise maze
    // Define lighting
    // Spawn enemy
}

void MainScene::update(float dt)
{
}

void MainScene::cleanup()
{
}
