#pragma once

#include "MicroGlut.h"
#include "VectorUtils4.h"
#include "Constants.h"
#include "Debug.h"

#include <unordered_map>
#include <string>

void glutClose(); //forward declare
class Input
{
public:
    static void init();
    static void update(float dt);
    static void on_mouse_move(int x, int y);

    static vec2 get_mouse_delta();
    
    static bool get_action(const std::string& action);

private:
    static vec2 deltaMousePos; // how much the mouse has moved in x,y
    static bool lockMouse;           // whether should keep mouse in center

    static std::unordered_map<std::string, char> inputMapping; //stores mapping of action to keypress
};