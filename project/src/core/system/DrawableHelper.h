#pragma once

#include <string>
#include "TextureManager.h"
#include "../entity/Drawable.h"
/*
    Class that provides helper functions to create drawables.
    Includes material helpers as well
    Should not have state, ie functions are static
*/

class DrawableHelper
{
public:
    static Material *create_material(std::string name, ShaderProg shaderProg, vec3 albedo, vec4 specular);
    static Material *attach_texture_to_material(Material *m, int idx, std::string textureName);

    static Drawable *create_from_model(std::string path, Transform t); // path relative from MODELS_PATH
    static Drawable *create_sphere(Transform t);
    static Drawable *create_cube(Transform t);
    static Drawable *create_plane(Transform t);
};


//shorthand for easy usage
typedef DrawableHelper dh;