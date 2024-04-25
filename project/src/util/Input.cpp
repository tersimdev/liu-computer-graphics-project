#include "Input.h"

vec2 Input::deltaMousePos = vec2(0, 0);
bool Input::lockMouse = true;
std::unordered_map<std::string, char> Input::inputMapping = std::unordered_map<std::string, char>();
std::unordered_map<char, char> Input::prevState = std::unordered_map<char, char>();

void Input::init()
{
    // define input map here
    inputMapping["exit"] = EXIT_KEY_1;
    inputMapping["exit2"] = EXIT_KEY_2;

    inputMapping["fwd"] = 'w';
    inputMapping["bck"] = 's';
    inputMapping["left"] = 'a';
    inputMapping["right"] = 'd';

    inputMapping["up"] = 'e';
    inputMapping["down"] = 'q';

    inputMapping["shoot"] = 'f';

    inputMapping["n"] = 'n';
    inputMapping["m"] = 'm';
}
void Input::update(float dt)
{
    /*
        Check if escape key pressed.
        0x1B is VK_ESCAPE (tested on windows)
    */
    if (get_action("exit", false))
    {
        debug_log("Exit button pressed, terminating\n");
        glutExit();
        exit(0);
    }
    if (get_action("exit2", false))
    {
        debug_log("Toggled mouse lock\n");
        lockMouse = !lockMouse;
    }
    if (lockMouse)
        glutWarpPointer(RES_X / 2, RES_Y / 2);
}
void Input::on_mouse_move(int x, int y)
{
    // debug_log("Mouse Coords: %d %d\n", x, y);
    if (!lockMouse)
        deltaMousePos = {0, 0};
    else
    {
        deltaMousePos.x = x - RES_X / 2;
        deltaMousePos.y = y - RES_Y / 2;
    }
}

vec2 Input::get_mouse_delta()
{
    return deltaMousePos;
}

bool Input::get_action(const std::string &action, bool hold)
{
    if (inputMapping.find(action) == inputMapping.end())
    {
        debug_error("Input Mapping has no action %s, try editing Input.h init().\n", action.c_str());
        return false;
    }
    char isDown = glutKeyIsDown(inputMapping[action]);
    char prev = prevState[inputMapping[action]];  
    prevState[inputMapping[action]] = isDown;
    if (hold && isDown)
    {
        //debug_log("%s (%d) key held\n", action.c_str(), inputMapping[action]);
        return true;
    }
    else if (!hold && isDown && !prev) //changed state
    {
        debug_log("%s (%d) key pressed\n", action.c_str(), inputMapping[action]);
        return true;
    }
    return false;
}

void Input::set_lock_mouse(bool lock)
{
    lockMouse = lock;
}
