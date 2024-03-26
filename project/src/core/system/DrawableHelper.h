#pragma once

#include <string>
#include "TextureManager.h"
#include "../entity/Drawable.h"
/*
    Class that provides helper functions to create drawables.
    Should not have state, ie functions are static
*/
class DrawableHelper
{
public:
    static Drawable* create_from_model(std::string path, Transform t); //path relative from MODELS_PATH
    static Drawable* create_sphere(Transform t);
    static Drawable* create_cube(Transform t); 
    static Drawable* create_plane(Transform t);
};