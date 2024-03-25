#include "Input.h"

void Input::init()
{
    // define input map here
    inputMapping["fwd"] = 'w';
    inputMapping["bck"] = 's';
    inputMapping["left"] = 'a';
    inputMapping["right"] = 'd';

    inputMapping["up"] = 'e';
    inputMapping["down"] = 'q';
}
void Input::update(float dt)
{
    /*
        Check if escape key pressed.
        0x1B is VK_ESCAPE (tested on windows)
    */
    if (glutKeyIsDown(EXIT_KEY_1) || glutKeyIsDown(EXIT_KEY_2))
    {
        debug_log("Exit button pressed, terminating\n");
        glutClose();
        exit(0);
    }
    if (lockMouse)
        glutWarpPointer(RES_X / 2, RES_Y / 2);
}
void Input::on_mouse_move(int x, int y)
{
    // debug_log("Mouse Coords: %d %d\n", x, y);
    deltaMousePos.x = x - RES_X / 2;
    deltaMousePos.y = y - RES_Y / 2;
}

vec2 Input::get_mouse_delta()
{
    return deltaMousePos;
}

bool Input::get_action(const std::string &action)
{
    if (inputMapping.find(action) == inputMapping.end())
    {
        debug_error("Input Mapping has no action %s, try editing Input.h init().\n", action.c_str());
        return false;
    }
    if (glutKeyIsDown(inputMapping[action]))
    {
        //debug_log("%s (%d) key pressed\n", action.c_str(), inputMapping[action]);
        return true;
    }
    return false;
}