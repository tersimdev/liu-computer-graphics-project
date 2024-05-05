#pragma once

/*
    Header file containing all global defines
*/

#define GAME_TITLE "TSBK07 Project"
#define AUTHOR "Terence & Peri"
#define RES_X 1024
#define RES_Y 576
#define EXIT_KEY_1 0x1B
#define EXIT_KEY_2 '0'
#define STARTING_SCENE "main"
#define PLAYER_START_POS {0, 0, 0}

//physics config here ///////////////////////////////////
//////////////////////////////////////////////////////////
#define GRAVITY -9.81
#define PHYS_ESPILON 0.0001
#define PHYS_TIMESTEP 0.02

//graphics config here ///////////////////////////////////
//////////////////////////////////////////////////////////

#define CLEAR_COL 0.0, 0.0, 0.0, 0
#define SHADER_PATH "./shaders/"
#define TEXTURES_PATH "./asset/texture/"
#define MODELS_PATH "./asset/model/"
#define MAX_TEXTURES 8
#define MAX_POINT_LIGHTS 16
#define INITIAL_FOV 45.0f
#define P_Z_NEAR 0.1f
#define P_Z_FAR 20.f
#define CAM_MAX_LOOK_UP 0.95f
#define CAM_DEADZONE 0.1f
#define DEFAULT_CAM_DIR { 0, 0, -1 }

//define hardcorded strings here! ////////////////////////
#define HELLO_WORLD "Hello World!"

/////////////////////////////////////////////////////////