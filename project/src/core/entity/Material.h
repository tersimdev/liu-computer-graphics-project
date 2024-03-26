#pragma once
#include "VectorUtils4.h"
#include "../../util/Constants.h"
#include "ShaderProg.h"

typedef struct _Material
{
    vec3 albedo; //defines color of ambient and diffuse
    vec4 specular; //rgb defines color of specular, a specifies exponent

    ShaderProg shaderProg; //program to use
    GLuint textures[MAX_TEXTURES]; //-1 for inactive
    int textureBitmask; //use bits to select which texture active
} Material;