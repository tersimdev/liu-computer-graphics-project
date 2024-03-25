#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"

#define LABNUM "3-1"

// Globals
Model *ground;
Model *m1, *m2;
GLuint myTex1, myTex2;
mat4 trans, rot, scale, identity, model;
float phi = 0;

// helper funcs defined below
Model *createGroundPlane(float groundSize, float uvTiles);

// projection matrix
#define P_NEAR 1.0
#define P_FAR 30.0
#define P_RIGHT 0.5
#define P_LEFT -0.5
#define P_TOP 0.5
#define P_BOTTOM -0.5
mat4 proj;
// camera variables
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
    m1 = LoadModel("models/bunny.obj");
    m2 = LoadModel("models/teapot.obj");
    ground = createGroundPlane(100.0f, 20.0f);
    // load textures
    LoadTGATextureSimple("textures/grass.tga", &myTex1);
    LoadTGATextureSimple("textures/conc.tga", &myTex2);
    printError("loading models/textures");
    // init matrices
    identity = mat4(1);
    model = identity;

    // load proj matrix
    proj = frustum(P_LEFT, P_RIGHT, P_BOTTOM, P_TOP, P_NEAR, P_FAR);
    // set up camera
    camPos = vec3(0, 2, 5);
    camDir = normalize((vec3(0) - camPos)); // look at origin
    camUp = vec3(0, 1, 0);

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab" LABNUM ".vert", "lab" LABNUM ".frag");
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
    float spinSpeed = 1.f;
    phi += dt * spinSpeed; // take note that this is radians
    if (phi >= 2 * M_PI)
        phi = 0; // reset phi when over 360 deg
                 // printf("phi: %.5f\n", phi);

    // Update model matrixes
    // TODO rotate bunny

    // Calculate view matrix based on camera vectors
    view = lookAtv(camPos, camPos + camDir, camUp);

    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(program, "elapsedTime"), t / 1000.0f);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw first model
    trans = T(0, 1, 1);
    rot = Ry(phi);
    scale = S(1.f);
    model = trans * rot * scale;
    glBindTexture(GL_TEXTURE_2D, myTex1);                                               // Select Texture
    glActiveTexture(GL_TEXTURE0);                                                       // activate texture 0 incase
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, model.m); // set model matrix
    DrawModel(m1, program, "in_Position", "in_Normal", "in_TexCoord");

    // draw second model
    trans = T(0, 1, -2);
    rot = Ry(phi * 3);
    scale = S(0.2f);
    model = trans * rot * scale;
    glBindTexture(GL_TEXTURE_2D, myTex2);                                               // Select Texture
    glActiveTexture(GL_TEXTURE0);                                                       // activate texture 0 incase
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, model.m); // set model matrix
    DrawModel(m2, program, "in_Position", "in_Normal", "in_TexCoord");

    // draw ground
    glBindTexture(GL_TEXTURE_2D, myTex1);                                               // Select Texture
    glActiveTexture(GL_TEXTURE0);                                                       // activate texture 0 incase
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, identity.m); // set model matrix
    DrawModel(ground, program, "in_Position", "in_Normal", "in_TexCoord");

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab " LABNUM " (Terence and Peri)");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

/* Helper functions below */ //////////////////////////////

Model *createGroundPlane(float groundSize, float uvTiles)
{
    vec3 vertices[] =
        {{-groundSize, 0.0f, -groundSize},
         {-groundSize, 0.0f, groundSize},
         {groundSize, -0.0f, -groundSize},
         {groundSize, -0.0f, groundSize}};
    vec3 normals[] =
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}};
    vec2 texCoords[] =
        {{0.0f, 0.0f},
         {0.0f, uvTiles},
         {uvTiles, 0.0f},
         {uvTiles, uvTiles}};
    GLuint indices[] = {0, 1, 2, 1, 3, 2};

    Model* ret = LoadDataToModel(vertices, normals, texCoords, NULL, indices, 4,6);
    return ret;
}
