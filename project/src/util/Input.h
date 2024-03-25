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
    void init();
    void update(float dt);
    void on_mouse_move(int x, int y);

    vec2 get_mouse_delta();
    
    bool get_action(const std::string& action);

private:
    vec2 deltaMousePos = vec2(0, 0); // how much the mouse has moved in x,y
    bool lockMouse = true;           // whether should keep mouse in center

    std::unordered_map<std::string, char> inputMapping; //stores mapping of action to keypress
};