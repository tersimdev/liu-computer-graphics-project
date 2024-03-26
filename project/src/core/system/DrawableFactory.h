#pragma once

#include "TextureManager.h"
#include "../entity/Drawable.h"
/*
    Class that provides helper functions to create drawables.
    Should not have state, ie functions are static
*/
class DrawableFactory
{
    static Drawable* create_from_model()
};