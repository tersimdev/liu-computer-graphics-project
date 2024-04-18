#pragma once

#include "VectorUtils4.h"
#include "../util/Debug.h"
#include "../util/Input.h"

//todo abstract movement from camera, have it be controlled by "Player" 

class Camera
{
public:
    void init();
    void update(float dt);
    void reset();
    
    void set_move_speed(float speed);
    void set_mouse_sens(float sens);
    void set_pos(vec3 pos);
    void set_dir(vec3 dir);

    vec3 get_pos();
    vec3 get_lookat();
    vec3 get_dir();
    vec3 get_up();

    void example_movement(float dt);
    void move_cam(vec3 direction, float dt);
    vec3 move_pos(vec3 pos, vec3 dir, float dt);
    void rotate_cam(vec2 delta,  float dt);
private:
    vec3 camPos, camDir, camUp;
    vec3 _camFront, _camRight; // tmp variables

    /* config vars */
    float camMoveSpeed = 0.01;
    float mouseSens = 0.001f;
};