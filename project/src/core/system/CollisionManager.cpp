#include "CollisionManager.h"
#include <algorithm>

void CollisionManager::init()
{
}

void CollisionManager::update(float dt)
{
}

void CollisionManager::cleanup()
{
}

void CollisionManager::register_collider(Collider *col)
{
    colliders.push_back(col);
}

void CollisionManager::unregister_collider(Collider *col)
{
    colliders.erase(std::remove(colliders.begin(), colliders.end(), col), colliders.end());
}

ColResult CollisionManager::check_sphere_sphere(SphereCollider *c1, SphereCollider *c2)
{
    vec3 dir = c1->get_position() - c2->get_position();
    float dist = Norm(dir);
    float radii = (c1->radius + c2->radius); 
    ColResult res = { false, vec3(0) };
    if (dist <= radii)
    {   
        res.overlap = true;
        //calculate delta
        if (dist < EPSILON)
            res.delta = radii * vec3(0,1,0); //arbitary direction
        else
            res.delta = (dist - radii) * (dir / dist)
    }
    return res;
}

ColResult CollisionManager::check_sphere_plane(SphereCollider *c1, PlaneCollider *c2)
{
    ColResult res = { false, vec3(0) };
     return res;
}
