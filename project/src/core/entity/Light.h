#pragma once
#include "VectorUtils4.h"
#include "../../util/Constants.h"

enum LightType
{
    DIR,
    POINT
};

typedef struct _LightSource
{
    LightType type;   // directional, point
    vec4 color;     // alpha for intensity/attenuation
    vec3 position;  // treated as direction for directional
} Light;