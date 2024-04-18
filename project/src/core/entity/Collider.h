#pragma once

#include <vector>
#include "VectorUtils4.h"

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
*/

class Collider
{
public:
    Collider(ColliderType coltype, vec3 pos) : coltype(coltype), position(pos) {}
    virtual ~Collider() = default;

    void move_position(vec3 delta) { position += delta; }
    void set_position(vec3 pos) { position = pos; }

    vec3 get_position() { return position; }
    ColliderType get_collider_type() { return coltype; }
protected:
    ColliderType coltype;
    vec3 position;
};