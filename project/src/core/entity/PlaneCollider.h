#pragma once

#include "Collider.h"

class PlaneCollider : public Collider
{
public:
    PlaneCollider(vec3 pos, vec3 normal, vec3 tangentX, vec3 tangentY, vec2 size) 
    : Collider(ColliderType::PLANE, pos), normal(normal), tangentX(tangentX), tangentY(tangentY), size(size) 
    {
    }
    PlaneCollider(vec3 pos, vec3 botLeft, vec3 topLeft, vec3 botRight) 
    : Collider(ColliderType::PLANE, pos)
    {
        //UNTESTED!
        vec3 bot = botRight - botLeft;
        vec3 left = topLeft - botLeft;
        normal = normalize(cross(bot, left));
        size.x = Norm(bot) * 0.5f;
        size.y = Norm(left) * 0.5f;
        tangentX = normalize(bot);
        tangentY = normalize(left);
    }
    ~PlaneCollider() = default;

    vec3 normal;
    vec3 tangentX; //if normal was z axis, xBound would be the horizontal bound 
    vec3 tangentY; //if normal was z axis, yBound would be the vertical bound
    vec2 size; //size along tangents
};