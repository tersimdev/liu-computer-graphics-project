#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"

// Globals
Model *m1, *m2;
GLuint myTex1, myTex2;
mat4 trans, rot, scale, identity, model;
float phi = 0;
const double PI = 3.14159;

// projection matrix
#define P_NEAR 1.0
#define P_FAR 30.0
#define P_RIGHT 0.5
#define P_LEFT -0.5
#define P_TOP 0.5
#define P_BOTTOM -0.5
mat4 proj;

//camera variables
vec3 camPos, camDir, camUp;
mat4 view;

// target update time in frames per second
unsigned int targetFPS = 60;

// total time elapsed
GLfloat t = 0;

// Reference to *current* shader program
GLuint program = -1;

void init(void)
{
    dumpInfo();

    // load models
    m1 = LoadModel("bunnyplus.obj");
    m2 = LoadModel("teapot.obj");
    // load textures
    LoadTGATextureSimple("grass.tga", &myTex1);
    LoadTGATextureSimple("conc.tga", &myTex2);
    printError("loading models/textures");
    // init matrices
    identity = mat4(1);
    model = identity;
    
    //load proj matrix
    proj = frustum(P_LEFT, P_RIGHT, P_BOTTOM, P_TOP, P_NEAR, P_FAR);
    //set up camera
    camPos = vec3(0, 2, 4);
    camDir = normalize((vec3(0)-camPos)); //look at origin
    camUp = vec3(0,1,0);

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab2-extra.vert", "lab2-extra.frag");
    printError("init shader");

    // Set static shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMtx"), 1, GL_TRUE, proj.m);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    printError("init uniforms");

    // Set display refresh, 1000ms / FPS
    glutRepeatingTimer(1000 / targetFPS);
}

void display(void)
{
    // Get delta time dt in seconds
    GLfloat dt = t; // set to previous t
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    dt = (t - dt) / 1000; // convert to s
    // printf("DT: %.5f\n", dt); //track frame time


    // Update things before rendering

    // Calculate a phi to rotate over time
    float spinSpeed = 0.2f;
    phi += dt * spinSpeed; // take note that this is radians
    if (phi >= 2 * PI)
        phi = 0; // reset phi when over 360 deg
                 // printf("phi: %.5f\n", phi);

    // Update model matrixes
    //TODO rotate bunny

    // Calculate view matrix based on camera vectors
    view = lookAtv(camPos, camPos + camDir, camUp);

    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(program, "elapsedTime"), t / 1000.0f);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //draw first model
    rot = Ry(45);
    model = rot;
    glBindTexture(GL_TEXTURE_2D, myTex1);      // Select Texture
    glActiveTexture(GL_TEXTURE0);              // activate texture 0 incase
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, model.m); //set model matrix
    DrawModel(m1, program, "in_Position", "in_Normal", "in_TexCoord");

    //draw second model
    trans = T(0,-1,-3);
    //rot = Ry(phi*3);
    scale = S(0.3f);
    model = trans * scale;
    glBindTexture(GL_TEXTURE_2D, myTex2);      // Select Texture
    glActiveTexture(GL_TEXTURE0);              // activate texture 0 incase
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, model.m); //set model matrix
    DrawModel(m2, program, "in_Position", "in_Normal", "in_TexCoord");

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ni hao");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

/*
Extra:
Added some displacement in vertex shader using texcoords and time
*/ 
