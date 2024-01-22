#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// Globals
Model *m;
GLuint myTex;
GLfloat myMatrix[] =
    {1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f};
float phi = 0;
const double PI = 3.14159;

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
    //load textures
    LoadTGATextureSimple("maskros512.tga", &myTex);
    printError("loading models/textures");

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab2-2.vert", "lab2-2.frag");
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

    //Set static shader uniforms
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

    float spinSpeed = 1.f;
    phi += dt * spinSpeed; // take note that this is radians
    if (phi >= 2 * PI)
        phi = 0; // reset phi when over 360 deg
                 // printf("phi: %.5f\n", phi);

    // Update things before rendering
    // Calculate rotation matrixes

    // Y rotation
    myMatrix[0] = cosf(phi);
    myMatrix[2] = -sinf(phi);
    myMatrix[8] = sinf(phi);
    myMatrix[10] = cosf(phi);

    //-X rotation
    // myMatrix[5] = cosf(-phi);
    // myMatrix[6] = -sinf(-phi);
    // myMatrix[9] = sinf(-phi);
    // myMatrix[10] = cosf(-phi);

    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
    glUniform1f(glGetUniformLocation(program, "elapsedTime"), t / 1000.0f);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(bunnyVertexArrayObjID); // Select VAO
    glBindTexture(GL_TEXTURE_2D, myTex); // Select Texture
    glActiveTexture(GL_TEXTURE0); //activate texture 0 incase
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

How are the textures coordinates mapped on the bunny? Can you see how they vary over the model?
Ans: Based on the texCoords, where bottom left of texture is at (0,0) and top right at (1,1)

How can you make a texture repeat multiple times over the bunny?
Ans: Tile / Scale the UV

Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. 
What? (No, it is not that we must avoid re-uploading from CPU. The texture object is on the GPU!)
Ans: The texUnit acts as "pointer". Instead of pasing several thousand pixels, we can pass a single number to refer to the texture.
The limited resource is likely CPU time or RAM amount. Textures are mostly stored on VRAM and can be referred to by their IDs.

*/
