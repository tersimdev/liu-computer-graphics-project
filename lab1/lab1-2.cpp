#include "GL_utilities.h"
#include "MicroGlut.h"

// Globals
// Data would normally be read from files
GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

GLfloat myMatrix[] =
    {
        1.0f, 0.0f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
    // vertex buffer object, used for uploading the geometry
    unsigned int vertexBufferObjID;
    // Reference to shader program
    GLuint program;

    dumpInfo();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab1-2.vert", "lab1-2.frag");
    printError("init shader");

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertexBufferObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // Set shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

    // End of upload of geometry

    printError("init arrays");
}

void display(void)
{
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID); // Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);    // draw object

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("GL3 white triangle example");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

//Questions:

//What is the purpose of the "in", "out" and "uniform" modifiers?
//Ans: in is used for the vertex shader to get data using the buffer from the cpu to gpu
//    out is used for the  fragment shader to get the colour of the pixel
//     uniform is used to pass variables from the cpu to the gpu dynamically
//
//What is the output of the fragment shader?
//Ans: Colour of the fragment

//What does the function glUniformMatrix4fv do?
//Ans: Set the value of the uniform variable in the shader