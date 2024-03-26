#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"

#include "../system/DrawableHelper.h"

/*
    Defines a player object. Handles logic like shooting input, movement, health
    Composes of entities like Drawables, colliders 
*/

class Player : public Thing
{
public:
    void init() override;
    void update(float dt) override;
    void cleanup() override;

    Drawable* get_drawable();
private:
    Drawable* drawable;
};