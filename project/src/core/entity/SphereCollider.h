#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
    SphereCollider(vec3 pos, float radius) 
    : Collider(ColliderType::SPHERE, pos), radius(radius) {}
    ~SphereCollider() = default;

    float radius;
};