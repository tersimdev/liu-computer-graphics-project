#pragma once

#include <string>
#include <unordered_map>
#include "../entity/Scene.h"
/*
    Class to manage scene transistions, and own all scene objects.
    Add new scenes in the init() function.
*/
class SceneManager
{
public:
    void init();
    void update(float dt);
    void cleanup();
    
    void change_scene(std::string sceneName);
private:
    std::unordered_map<std::string, Scene*> scenes;
    Scene* activeScene;
};