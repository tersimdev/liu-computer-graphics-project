#pragma once

#include "Collider.h"

class PlaneCollider : public Collider
{
public:
    PlaneCollider(vec3 pos, vec3 normal) 
    : Collider(ColliderType::PLANE, pos), normal(normal) {}
    ~PlaneCollider() = default;

    vec3 normal;
    vec2 size; //size along the two tangential axes
};