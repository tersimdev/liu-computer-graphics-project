#pragma once

/*
    Header file containing all global defines
*/

#define GAME_TITLE "TSBK07 Project"
#define AUTHOR "Terence & Peri"
#define RES_X 600
#define RES_Y 600
#define EXIT_KEY_1 0x1B
#define EXIT_KEY_2 '0'

//graphics config here ///////////////////////////////////
//////////////////////////////////////////////////////////

#define CLEAR_COL 0.2, 0.2, 0.5, 0
#define SHADER_PATH "./shaders/"
#define MAX_TEXTURES 8
#define MAX_POINT_LIGHTS 16

//for projection
#define P_NEAR 1.0
#define P_FAR 30.0
#define P_RIGHT 0.5
#define P_LEFT -0.5
#define P_TOP 0.5
#define P_BOTTOM -0.5

//define hardcorded strings here! ////////////////////////
#define HELLO_WORLD "Hello World!"

/////////////////////////////////////////////////////////