#include "CollisionManager.h"
#include <algorithm>
#include "../../util/Debug.h"
#include "../../util/Constants.h"

void CollisionManager::init()
{
}

// there are unchecked upcasts in this function, do ensure ColliderType matches expected derived class!
void CollisionManager::update(float dt)
{
    int n = colliders->size();
    Collider *c1, *c2;
    ColResult res;
    vec3 move;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            c1 = colliders->at(i);
            c2 = colliders->at(j);

            // if both have no rigidbody, dont bother for now
            if (no_rigidbodies(c1, c2))
                continue;

            if (c1->get_collider_type() == SPHERE && c2->get_collider_type() == SPHERE)
            {
                res = check_sphere_sphere((SphereCollider *)c1, (SphereCollider *)c2);
            }
            else if (c1->get_collider_type() == SPHERE && c2->get_collider_type() == PLANE)
            {
                res = check_sphere_plane((SphereCollider *)c1, (PlaneCollider *)c2);
            }
            else if (c1->get_collider_type() == PLANE && c2->get_collider_type() == SPHERE)
            {
                res = check_sphere_plane((SphereCollider *)c2, (PlaneCollider *)c1);
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

bool CollisionManager::no_rigidbodies(Collider *c1, Collider *c2)
{
    return c1->get_rigidbody() == nullptr && c2->get_rigidbody() == nullptr;
}

ColResult CollisionManager::check_sphere_sphere(SphereCollider *c1, SphereCollider *c2)
{
    vec3 dir = c2->get_position() - c1->get_position();
    float dist = Norm(dir);
    float radii = (c1->radius + c2->radius);
    ColResult res = {false, vec3(0)};
    if (dist + PHYS_ESPILON <= radii)
    {
        res.overlap = true;
        // calculate delta
        if (dist < PHYS_ESPILON)
            res.delta = radii * vec3(0, 1, 0); // arbitary direction
        else
            res.delta = (dist - radii) * (dir / dist);
        debug_log("Collided, resolved with %f %f %f\n", res.delta.x, res.delta.y, res.delta.z);
        resolve_collision(c1, c2, res.delta);
    }
    return res;
}

ColResult CollisionManager::check_sphere_plane(SphereCollider *c1, PlaneCollider *c2)
{
    // TODO
    ColResult res = {false, vec3(0)};
    return res;
}

void CollisionManager::resolve_collision(Collider *c1, Collider *c2, vec3 delta)
{
    Rigidbody *rb1 = c1->get_rigidbody(), *rb2 = c2->get_rigidbody();
    if (rb1 && rb2)
    {
        vec3 force = delta * 2;
        if (rb1->get_rbtype() == KINEMATIC && rb2->get_rbtype() == KINEMATIC)
        {
            c1->move_position(delta * 0.5f);
            c2->move_position(-delta * 0.5f);
        }
        else if (rb1->get_rbtype() == DYNAMIC && rb2->get_rbtype() == DYNAMIC)
        {
            //TODO, use physical formula?
            rb1->add_force(delta);
            rb2->add_force(-delta);
        }
        else if (rb1->get_rbtype() == DYNAMIC && rb2->get_rbtype() == KINEMATIC) 
        {
            c2->move_position(-delta);
        }
        else if (rb1->get_rbtype() == KINEMATIC && rb2->get_rbtype() == DYNAMIC) 
        {
            c1->move_position(delta);
        }
    }
    else if (rb1 == nullptr) // just move rb2
    {   
        c2->move_position(-delta);
    }
    else if (rb2 == nullptr) // just move rb1
    {
        c1->move_position(delta);
    }
}
