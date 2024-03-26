#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"

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

    void attach_drawable(Drawable* d);
private:
    Drawable* drawable;
};