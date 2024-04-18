#pragma once

#include <vector>
#include "../entity/Collider.h"
#include "../entity/SphereCollider.h"
#include "../entity/PlaneCollider.h"

typedef struct _ColResult
{
    bool overlap;
    vec3 delta; // how much either collider to move to resolve
} ColResult;

/*
    Class to manage all collision detections and resolutions
*/
class CollisionManager
{
public:
    void init();
    void update(float dt);
    void cleanup();
    void register_collider(Collider *col);
    void unregister_collider(Collider *col);

private:
    ColResult check_sphere_sphere(SphereCollider *c1, SphereCollider *c2);
    ColResult check_sphere_plane(SphereCollider *c1, PlaneCollider *c2);

    const float EPSILON = 0.001f;

    std::vector<Collider *> colliders;
};