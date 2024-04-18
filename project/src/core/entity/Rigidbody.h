#pragma once

#include <vector>
#include "VectorUtils4.h"
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
    gravScale(1), vel({0,0,0}), accel({0,0,0}) {}
    virtual ~Rigidbody() = default;

    vec3 update(float dt, vec3 pos)
    {
        //apply gravity
        //disabled for now, no floor
        // accel.y -= GRAVITY * gravScale;
        // if (accel.y < PHYS_ESPILON)
        //     accel.y = 0;
        
        //apply euler integration
        vel += accel * dt;
        pos += vel * dt;
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

    void set_grav_scale(float gs) { gravScale = gs; }

private:
    RigidbodyType rbtype;
    float mass;
    float gravScale;

    vec3 vel, accel;
};