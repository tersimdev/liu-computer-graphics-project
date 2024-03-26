#include "SceneManager.h"

/* Includes for all scenes here */
#include "../scene/MainScene.h"
#include "../../util/Constants.h"
#include "../../util/Debug.h"

void SceneManager::init(GraphicsManager* graphicsMgr)
{
    this->graphicsMgr = graphicsMgr;

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
    activeScene->init(this);
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
    activeScene->init(this);
}

GraphicsManager *SceneManager::get_graphics_mgr()
{
    return graphicsMgr;
}