#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/Material.h"
#include "../entity/Light.h"
#include "../../util/Camera.h"
#include <vector>

#include "../system/DrawableHelper.h"

/*
    Base class defining a scene construct. Inherit to create new scenes.
*/
class Scene 
{
public:
    virtual ~Scene() = default;
    virtual void init(Camera* camera) = 0; //abstract, as this is empty scene
    virtual void update(float dt);
    virtual void cleanup();

    std::vector<Light*>& get_lights();
    std::vector<Drawable*>* get_drawables();
    std::vector<Thing*>* get_things();
protected:
    std::vector<Light*> lights; //0th is dirLight
    std::vector<Drawable*> drawables;
    std::vector<Thing*> things;
};