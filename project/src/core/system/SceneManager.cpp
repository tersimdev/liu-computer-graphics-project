#include "SceneManager.h"

/* Includes for all scenes here */
#include "../scene/MainScene.h"
#include "../../util/Constants.h"
#include "../../util/Debug.h"

void SceneManager::init(GraphicsManager* graphicsMgr, CollisionManager* colMgr)
{
    this->graphicsMgr = graphicsMgr;
    this->colMgr = colMgr;
    /**
     * ADD YOUR SCENES HERE.
     * DEFINE STARTING SCENE IN CONSTANTS
     */

    scenes["main"] = new MainScene();
    if (scenes.find(STARTING_SCENE) == scenes.end())
    {
        debug_error("STARTING SCENE IS INVALID! (%s) Please fix in Constants.h\n", STARTING_SCENE);
        return;
    }
    activeScene = scenes[STARTING_SCENE];
    init_active_scene();
}
void SceneManager::update(float dt)
{
    if (activeScene)
        activeScene->update(dt);
}
void SceneManager::cleanup()
{
    for (auto [k, v] : scenes)
    {
        v->cleanup();
        delete v;
    }
    scenes.clear();
}

void SceneManager::change_scene(std::string sceneName)
{
    if (scenes.find(sceneName) == scenes.end())
    {
        debug_error("Invalid scene to change to (%s)", sceneName.c_str());
        return;
    }
    activeScene->cleanup();
    activeScene = scenes[sceneName];
    init_active_scene();
}

void SceneManager::init_active_scene()
{
    activeScene->init(graphicsMgr->get_camera());
    graphicsMgr->set_drawable_list(activeScene->get_drawables());
    graphicsMgr->reset_lights();
    std::vector<Light*> lights = activeScene->get_lights();
    int n = lights.size();
    if (n > 0) //take first as directional
        graphicsMgr->set_dir_light(lights[0]);
    
    //take the rest as point lights
    for (int i = 1; i < n; ++i)
    {
        if (i-1 >= MAX_POINT_LIGHTS) 
        {
            debug_warn("Number of point lights allowed excceeded (%d)\n", i-1);
            break;
        }
        graphicsMgr->set_point_light(i-1 ,lights[i]);
    }

    colMgr->set_collider_list(activeScene->get_colliders());
}
