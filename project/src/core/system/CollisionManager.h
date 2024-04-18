#pragma once

#include <vector>
#include "../entity/Collider.h"
#include "../entity/SphereCollider.h"
#include "../entity/PlaneCollider.h"

typedef struct _ColResult
{
    bool overlap;
    vec3 delta; // how much collider 1 should move to resolve
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
    void set_collider_list(std::vector<Collider *>*);

private:
    bool no_rigidbodies(Collider* c1, Collider* c2); //helper to check if both colliders have no rigidbody, ie both static
    ColResult check_sphere_sphere(SphereCollider *c1, SphereCollider *c2);
    ColResult check_sphere_plane(SphereCollider *c1, PlaneCollider *c2);

    void resolve_collision(Collider *c1, Collider *c2, vec3 delta);

    std::vector<Collider *>* colliders;
};