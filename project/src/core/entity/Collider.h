#pragma once

#include <vector>
#include "VectorUtils4.h"
#include "Rigidbody.h" 

enum ColliderType
{
    SPHERE,
    PLANE,

    NUM_COL_TYPES
};

/*
    Specifies a collider base class.
    Values for coltype are set in derived classes, they are immutable in runtime.
    Owner of collider object is responsible for updating position.
    Colliders only move to unoverlap if there is a rigidbody attached.
*/

class Collider
{
public:
    Collider(ColliderType coltype, vec3 pos) : coltype(coltype), position(pos), rigidbody(nullptr), active(true) {}
    virtual ~Collider() = default;

    void move_position(vec3 delta) { position += delta; }
    void set_position(vec3 pos) { position = pos; }
    void set_rigidbody(Rigidbody* rb) { rigidbody = rb; }
    void set_active(bool b) { active = b; }

    ColliderType get_collider_type() { return coltype; }
    vec3 get_position() { return position; }
    Rigidbody* get_rigidbody() { return rigidbody; }

protected:
    ColliderType coltype;
    vec3 position;
    Rigidbody* rigidbody;
    bool active;
};