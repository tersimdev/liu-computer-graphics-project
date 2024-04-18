#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
    SphereCollider(float radius) 
    : Collider(ColliderType::SPHERE), radius(radius) {}
    ~SphereCollider() = default;

    float radius;
};