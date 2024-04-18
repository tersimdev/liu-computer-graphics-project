#include "CollisionManager.h"
#include <algorithm>
#include "../../util/Debug.h"

void CollisionManager::init()
{
}

void CollisionManager::update(float dt)
{
    int n = colliders->size();
    Collider *c1, *c2;
    ColResult res;
    vec3 move;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i+1; j < n; ++j)
        {
            c1 = colliders->at(i);
            c2 = colliders->at(j);

            if (c1->get_collider_type() == SPHERE && c2->get_collider_type() == SPHERE)
            {
                res = check_sphere_sphere((SphereCollider*)c1, (SphereCollider*)c2); 
                if (res.overlap)
                {
                    move = (res.delta * 0.5f);
                    c1->move_position(-move);
                    c2->move_position(move);
                }                
            }
            else if (c1->get_collider_type() == SPHERE && c2->get_collider_type() == PLANE)
            {
                res = check_sphere_plane((SphereCollider*)c1, (PlaneCollider*)c2);
            }
            else if (c1->get_collider_type() == PLANE && c2->get_collider_type() == SPHERE)
            {
                res = check_sphere_plane((SphereCollider*)c2, (PlaneCollider*)c1);
            }
        }
    }
}

void CollisionManager::cleanup()
{
    // colliders belong to scene, so no need to delete here
}

void CollisionManager::set_collider_list(std::vector<Collider *> *list)
{
    colliders = list;
}

ColResult CollisionManager::check_sphere_sphere(SphereCollider *c1, SphereCollider *c2)
{
    vec3 dir = c1->get_position() - c2->get_position();
    float dist = Norm(dir);
    float radii = (c1->radius + c2->radius);
    ColResult res = {false, vec3(0)};
    if (dist + EPSILON <= radii)
    {
        res.overlap = true;
        // calculate delta
        if (dist < EPSILON)
            res.delta = radii * vec3(0, 1, 0); // arbitary direction
        else
            res.delta = (dist - radii) * (dir / dist);
        debug_log("Collided, resolved with %f %f %f\n", res.delta.x, res.delta.y, res.delta.z);
    }
    return res;
}

ColResult CollisionManager::check_sphere_plane(SphereCollider *c1, PlaneCollider *c2)
{
    //TODO
    ColResult res = {false, vec3(0)};
    return res;
}
