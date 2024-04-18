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
    vec3 dir = c2->get_position() - c1->get_position();
    float dist = dot(dir, c2->normal);
    ColResult res = {false, vec3(0)};
    if (dist + PHYS_ESPILON < c1->radius)
    {
        //vector pointing from center of sphere to nearest point on plane
        vec3 delta = (c2->normal * (c1->radius - dist)); 
        //nearest point on plane
        vec3 p = c1->get_position() - delta; 
        //vector from center of plane to nearest point
        vec3 cp = p - c2->get_position(); 
        //distance when projected onto x bounds
        float xdist = dot(cp, c2->tangentX) + PHYS_ESPILON;
        //distance when projected on y bound
        float ydist = dot(cp, c2->tangentY) + PHYS_ESPILON;
        
        // check if point is inside defined plane
        ////////////////// TODO, check if it works!
        if (xdist < c2->size.x || ydist < c2->size.y)
        {
            res.overlap = true;
            res.delta = delta;
        }
    }

    return res;
}

void CollisionManager::resolve_collision(Collider *c1, Collider *c2, vec3 delta)
{
    Rigidbody *rb1 = c1->get_rigidbody(), *rb2 = c2->get_rigidbody();
    if (rb1 && rb2)
    {
        if (rb1->get_rbtype() == KINEMATIC && rb2->get_rbtype() == KINEMATIC)
        {
            c1->move_position(delta * 0.5f);
            c2->move_position(-delta * 0.5f);
        }
        else if (rb1->get_rbtype() == DYNAMIC && rb2->get_rbtype() == DYNAMIC)
        {
            vec3 dir = normalize(delta);
            float v1 = Norm(rb1->get_vel());
            float v2 = Norm(rb2->get_vel());
            float m1 = rb1->get_mass();
            float m2 = rb2->get_mass();
            float e = ( rb1->get_elasticity() + rb2->get_elasticity() ) * 0.5f;
            float v1a = (v1 * (m1 - e * m2) + v2 * (1+e) * m2) / (m1 + m2);
            float v2a = (v2 * (m2 - e * m1) + v1 * (1+e) * m1) / (m1 + m2);
            rb1->set_vel(dir * (v1a)); 
            rb2->set_vel(-dir * (v2a));      
        }
        else if (rb1->get_rbtype() == DYNAMIC && rb2->get_rbtype() == KINEMATIC)
        {
            c2->move_position(-delta);
            rb1->set_vel(delta * 20); //temp
        }
        else if (rb1->get_rbtype() == KINEMATIC && rb2->get_rbtype() == DYNAMIC)
        {
            c1->move_position(delta);
            rb2->set_vel(-delta * 20); //temp
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
