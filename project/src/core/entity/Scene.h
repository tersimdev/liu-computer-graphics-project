#pragma once

#include "Drawable.h"
#include "Material.h"
#include "Light.h"

/*
    Abstract class defining a scene construct. Inherit to create new scenes.
*/
class Scene 
{
public:
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void cleanup() = 0;
};