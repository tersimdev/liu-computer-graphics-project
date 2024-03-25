#pragma once

#include "VectorUtils4.h"
#include "../util/Debug.h"
#include "../util/Input.h"

//todo abstract movement from camera, have it be controlled by "Player" 

class Camera
{
public:
    void init(Input *input);
    void update(float dt);
    void reset();
    void set_move_speed(float speed);
    
    vec3 get_pos();
    vec3 get_lookat();
    vec3 get_dir();
    vec3 get_up();

private:
    Input *input; // dependency

    vec3 camPos, camDir, camUp;
    vec3 _camFront, _camRight; // tmp variables

    /* config vars */
    float camMoveSpeed = 2;
    float camDeadzone = 0.1f;
    float maxLookUp = 0.95f;
    float mouseSens = 0.2f;

    // Camera keyboard movement (WASD)
    void move_cam(vec3 camFront, vec3 camRight, float dt);  
    // Camera mouse movement
    void rotate_cam(vec3 camRight, float dt);
};