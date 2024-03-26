#pragma once
#include "VectorUtils4.h"
#include "../../util/Constants.h"


typedef struct _LightSource
{
    int type;       // 0 for dir, 1 for point
    vec4 color;     // alpha for intensity/attenuation
    vec3 position;  // treated as direction for directional
} Light;