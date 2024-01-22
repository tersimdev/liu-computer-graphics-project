#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

// Globals
// Data would normally be read from files

// Ensure that vertices are in counter-clockwise order for correct culling 
GLfloat vertices[] =
    {
        // base 1
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        // base  2
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        // triangle red
        0.5f, -0.5f, -0.5f,
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,
        // triangle green
        -0.5f, -0.5f, -0.5f,
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
        // triangle blue
        0.5f, -0.5f, 0.5f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, -0.5f,
        // triangle yellow
        -0.5f, -0.5f, 0.5f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.5f};

GLfloat colors[] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, // base 1
        1, 1, 1, 1, 1, 1, 1, 1, 1, // base 2
        1, 0, 0, 1, 0, 0, 1, 0, 0, // red triangle
        0, 1, 0, 0, 1, 0, 0, 1, 0, // green triangle
        0, 0, 1, 0, 0, 1, 0, 0, 1, // blue triangle
        1, 1, 0, 1, 1, 0, 1, 1, 0  // yellow triangle

};

GLfloat myMatrix[] =
    {1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f};

const double PI = 3.14159;

float phi = 0;

// vertex array object
unsigned int vertexArrayObjID;

// target update time in frames per second
unsigned int targetFPS = 60;

// total time elapsed
GLfloat t = 0;

// Reference to *current* shader program
GLuint program = -1;

void init(void)
{
    // vertex buffer object, used for uploading the geometry
    unsigned int vertexBufferObjID;

    dumpInfo();


    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab1-5.vert", "lab1-5.frag");
    printError("init shader");

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertexBufferObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // Use a separate VBO for color
    // Ideally should use the same VBO and define a vertex struct
    glGenBuffers(1, &vertexBufferObjID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

    // Set constant shader uniforms

    // End of upload of geometry

    printError("init arrays");

    // Set display refresh, 1000ms / FPS
    glutRepeatingTimer(1000 / 60);
}

void display(void)
{
    // Get delta time dt in seconds
    GLfloat dt = t; // set to previous t
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    dt = (t - dt) / 1000; // convert to s
    // printf("DT: %.5f\n", dt); //track frame time

    float spinSpeed = 2;
    phi += dt * spinSpeed; // take note that this is radians
    if (phi >= 2 * PI)
        phi = 0; // reset phi when over 360 deg
    // printf("phi: %.5f\n", phi);

    // Update things before rendering
    // Calculate rotation matrixes

    //Y rotation
    // myMatrix[0] = cosf(phi);
    // myMatrix[2] = -sinf(phi);
    // myMatrix[8] = sinf(phi);
    // myMatrix[10] = cosf(phi);

    //-X rotation
    myMatrix[5] = cosf(-phi);
    myMatrix[6] = -sinf(-phi);
    myMatrix[9] = sinf(-phi);
    myMatrix[10] = cosf(-phi);


    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID); // Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 3 * 6); // draw object, second param is number of vertices

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

//Questions:
//What problems did you encounter while building the pyramid?
//Ans: Some triangles will be drawn ontop of other triangles.

//How do you change the facing of a polygon?
//Ans: Put it in counterclockwise order of the direction you want it to face, swap the order