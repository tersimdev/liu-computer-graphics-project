#pragma once

#include <vector>
#include "VectorUtils4.h"
#include "VectorUtils4Extra.h"
#include "../../util/Debug.h"
#include "../../util/Constants.h"

enum RigidbodyType
{
    KINEMATIC, //affected by collision, but position is set explicitly
    DYNAMIC, //affected by things like gravity, velocity, impulse
    //STATIC, //implicitly static when something has no rigidbody

    NUM_RB_TYPES
};

/*
    Specifies a rigidbody. Rigidbodies repsond to physics events like collision.
*/

class Rigidbody
{
public:
    Rigidbody(RigidbodyType rbtype, float mass) 
    : rbtype(rbtype), mass(mass), 
    gravScale(1), elasticity(0.7), dampening(0.1),
    vel({0,0,0}), accel({0,0,0}) 
    {}

    vec3 update(float dt, vec3 pos)
    {
        //apply gravity
        //disabled for now, no floor
        //if colliding with floor, need to stop gravity
        //accel.y = GRAVITY * gravScale;
        
        //apply euler integration
        vel += accel * dt;
        pos += vel * dt;

        //dampen accel and vel to act as friction
        vel = vel - vel*dampening*dt;
        if (NormSq(vel) < PHYS_ESPILON * PHYS_ESPILON)
            vel = vec3(0);
        if (NormSq(accel) < PHYS_ESPILON * PHYS_ESPILON)
            accel = vec3(0);
        return pos;
    }
    void add_force(vec3 force)
    {
        //asert mass is not 0
        if (mass < PHYS_ESPILON)
        {
            debug_error("Mass cannot be zero.\n");
            return;
        }
        accel = force / mass;
    }

    RigidbodyType get_rbtype() { return rbtype; }
    float get_mass() { return mass; }
    vec3 get_vel() { return vel; }
    float get_elasticity() { return elasticity; }

    void set_grav_scale(float gs) { gravScale = gs; }
    void set_elasticity(float e) { elasticity = e; }
    void set_vel(vec3 v) { vel = v; }
    void set_accel(vec3 a) { accel = a; }

private:
    RigidbodyType rbtype;
    float mass;
    float gravScale;
    float elasticity;
    float dampening; //acts as friction

    vec3 vel, accel;
};