#include "Camera.h"

void Camera::init(Input *input)
{
    this->input = input;
    reset();
}
void Camera::update(float dt)
{
    _camFront = camDir;
    _camFront.y = 0;
    _camRight = normalize(cross(camDir, camUp));
    _camRight.y = 0; // incase

    rotate_cam(_camRight, dt);
    move_cam(_camFront, _camRight, dt);
    //debug_log("pos %f %f %f\n", camPos.x, camPos.y, camPos.z);
}
void Camera::reset()
{
    camPos = {0, 0, 0};
    camDir = {0, 0, -1};
    camUp = {0, 1, 0};
}

void Camera::set_move_speed(float speed)
{
    camMoveSpeed = speed;
}

vec3 Camera::get_pos()
{
    return camPos;
}

vec3 Camera::get_lookat()
{
    return camPos + camDir;
}

vec3 Camera::get_dir()
{
    return camDir;
}

vec3 Camera::get_up()
{
    return camUp;
}

// Camera keyboard movement (WASD)
void Camera::move_cam(vec3 camFront, vec3 camRight, float dt)
{
    if (input->get_action("fwd"))
    {
        // move along the front direction (view igrnoring y)
        camPos += camFront * camMoveSpeed * dt;
    }
    else if (input->get_action("bck"))
    {
        // same as w but -1
        camPos += -camFront * camMoveSpeed * dt;
    }
    if (input->get_action("right"))
    {
        // calculate right and move along it
        camPos += camRight * camMoveSpeed * dt;
    }
    else if (input->get_action("left"))
    {
        // same as d but -1
        camPos += -camRight * camMoveSpeed * dt;
    }

    if (input->get_action("up"))
    {
        // move u in y
        vec3 up = vec3(0, 1, 0); // incase camUp is invalid
        camPos += up * camMoveSpeed * dt;
    }
    else if (input->get_action("down"))
    {
        // same as up but -1
        vec3 up = vec3(0, 1, 0); // incase camUp is invalid
        camPos += -up * camMoveSpeed * dt;
    }
}

// Camera mouse movement
void Camera::rotate_cam(vec3 camRight, float dt)
{
    vec2 deltaMouse = input->get_mouse_delta();
    if (deltaMouse.x > camDeadzone || deltaMouse.x < -camDeadzone)
    {
        camDir = ArbRotate(camUp, -deltaMouse.x * dt * mouseSens) * camDir;
        camDir = normalize(camDir);
    }
    if ((deltaMouse.y > camDeadzone && camDir.y > -maxLookUp) ||
        (deltaMouse.y < -camDeadzone && camDir.y < maxLookUp))
    {
        camDir = ArbRotate(camRight, -deltaMouse.y * dt * mouseSens) * camDir;
        camDir = normalize(camDir);
    }
}