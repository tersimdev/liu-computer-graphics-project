#pragma once

#include <string>
#include <unordered_map>
#include "../scene/Scene.h"
#include "TextureManager.h"
#include "GraphicsManager.h"
#include "CollisionManager.h"

/*
    Class to manage scene transistions, and own all scene objects.
    Add new scenes in the init() function.
*/
class SceneManager
{
public:
    void init(GraphicsManager* graphicsMgr, CollisionManager* colMgr);
    void update(float dt);
    void cleanup();
    void change_scene(std::string sceneName);
    void init_active_scene();
    
    GraphicsManager* get_graphics_mgr();
private:
    std::unordered_map<std::string, Scene*> scenes;
    Scene* activeScene;

    //dependency injections
    GraphicsManager* graphicsMgr;
    CollisionManager* colMgr;
};