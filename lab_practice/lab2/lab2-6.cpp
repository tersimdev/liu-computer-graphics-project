#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"

// Globals
Model *m;
GLuint myTex;
mat4 scale, rot, trans, model;
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

// vertex array object
unsigned int bunnyVertexArrayObjID;

// vertex buffer objects, used for uploading the geometry
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

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
    m = LoadModel("bunnyplus.obj");
    if (m == NULL)
    {
        printError("failed to load model");
        return;
    }
    // load textures
    LoadTGATextureSimple("dirt.tga", &myTex);
    printError("loading models/textures");
    
    //load proj matrix
    proj = frustum(P_LEFT, P_RIGHT, P_BOTTOM, P_TOP, P_NEAR, P_FAR);
    //set up camera
    camPos = vec3(0, 1, 3);
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
    program = loadShaders("lab2-6.vert", "lab2-6.frag");
    printError("init shader");

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glBindVertexArray(bunnyVertexArrayObjID);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glGenBuffers(1, &bunnyTexCoordBufferObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

    // VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

    // VBO for texture coords
    if (m->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices * 2 * sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));
    }

    // Set static shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMtx"), 1, GL_TRUE, proj.m);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0


    // End of upload of geometry

    printError("init arrays");

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
    if (phi >= 2 * PI)
        phi = 0; // reset phi when over 360 deg
                 // printf("phi: %.5f\n", phi);

    // Update model matrix
    scale = S(1);
    rot = Ry(phi);
    trans =  T(0,0,0);
    model = trans * rot * scale;

    // Calculate view matrix based on camera vectors
    view = lookAtv(camPos, camPos + camDir, camUp);

    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, model.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(program, "elapsedTime"), t / 1000.0f);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(bunnyVertexArrayObjID); // Select VAO
    glBindTexture(GL_TEXTURE_2D, myTex);      // Select Texture
    glActiveTexture(GL_TEXTURE0);             // activate texture 0 incase
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
Questions:

Was the difference big? If not, why?
(Gouraud and Phong)
Not much visible difference.
For Phong: The normals between vertices are interpolated. 
For Gouraud: The calculated shaded color between vertices are interpolated. 
*/ 
