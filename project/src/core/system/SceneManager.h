#pragma once

#include <string>
#include <unordered_map>
#include "../entity/Scene.h"
#include "TextureManager.h"
#include "GraphicsManager.h"

/*
    Class to manage scene transistions, and own all scene objects.
    Add new scenes in the init() function.
*/
class SceneManager
{
public:
    void init(GraphicsManager* graphicsMgr);
    void update(float dt);
    void cleanup();
    void change_scene(std::string sceneName);
    
    GraphicsManager* get_graphics_mgr();
private:
    std::unordered_map<std::string, Scene*> scenes;
    Scene* activeScene;

    //dependency injections
    GraphicsManager* graphicsMgr;
};