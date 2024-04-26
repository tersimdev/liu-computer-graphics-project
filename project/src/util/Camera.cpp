#include "Camera.h"

void Camera::init()
{
    reset();
}
void Camera::update(float dt)
{
    _camFront = camDir;
    _camFront.y = 0;
    _camRight = normalize(cross(camDir, camUp));
    _camRight.y = 0; // incase

    // debug_log("pos %f %f %f\n", camPos.x, camPos.y, camPos.z);
}
void Camera::reset()
{
    camPos = {0, 0, 0};
    camDir = DEFAULT_CAM_DIR;
    camUp = {0, 1, 0};
}

void Camera::set_move_speed(float speed)
{
    camMoveSpeed = speed;
}

void Camera::set_mouse_sens(float sens)
{
    mouseSens = sens;
}

void Camera::set_pos(vec3 pos)
{
    camPos = pos;
}

void Camera::set_dir(vec3 dir)
{
    camDir = normalize(dir);
}

void Camera::set_up(vec3 up)
{
    camUp = normalize(up);
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
void Camera::move_cam(vec3 direction, float dt)
{
    if (NormSq(direction - vec3(0)) > 0.1) //not zero
        direction = normalize(direction);
    camPos += -direction.z * _camFront * camMoveSpeed * dt;
    camPos += direction.x * _camRight * camMoveSpeed * dt;
    camPos += direction.y * camUp * camMoveSpeed * dt;
}

//moves a given position vector
vec3 Camera::move_pos(vec3 pos, vec3 dir, float dt)
{
    if (NormSq(dir - vec3(0)) > 0.1) //not zero
        dir = normalize(dir);
    pos += -dir.z * _camFront * camMoveSpeed * dt;
    pos += dir.x * _camRight * camMoveSpeed * dt;
    pos += dir.y * camUp * camMoveSpeed * dt;

    return pos;
}

// Camera mouse movement
void Camera::rotate_cam(vec2 delta, float dt)
{
    if (delta.x > CAM_DEADZONE || delta.x < -CAM_DEADZONE)
    {
        camDir = ArbRotate(camUp, -delta.x * dt * mouseSens) * camDir;
        camDir = normalize(camDir);
    }
    if ((delta.y > CAM_DEADZONE && camDir.y > -CAM_MAX_LOOK_UP) ||
        (delta.y < -CAM_DEADZONE && camDir.y < CAM_MAX_LOOK_UP))
    {
        camDir = ArbRotate(_camRight, -delta.y * dt * mouseSens) * camDir;
        camDir = normalize(camDir);
    }
}

void Camera::example_movement(float dt)
{
    // wasd qe movement
    vec3 dir = {0,0,0};
    if (Input::get_action("fwd"))
        dir.z = -1;
    else if (Input::get_action("bck"))
        dir.z = 1;
    if (Input::get_action("right"))
        dir.x = 1;
    else if (Input::get_action("left"))
        dir.x = -1;
    if (Input::get_action("up"))
        dir.y = 1;
    else if (Input::get_action("down"))
        dir.y = -1;
    move_cam(dir, dt);

    // rotation
    vec2 deltaMouse = Input::get_mouse_delta();
    rotate_cam(deltaMouse, dt);
}
