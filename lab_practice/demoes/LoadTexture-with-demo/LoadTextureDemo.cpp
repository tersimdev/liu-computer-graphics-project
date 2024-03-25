// Test of four texture loaders.

// gcc LoadTextureDemo.c ../common/*.c -lGL -o LoadTextureDemo -I../common
// plus the rest... something like -lz ../LoadTexture/*.c -ILoadTexture

//uses framework Cocoa
//uses framework OpenGL
#include "MicroGlut.h"
#include "GL_utilities.h"

#include "LoadTexture.h"

// Globals
// Data would normally be read from files
GLfloat vertices[] = { -0.5f,-0.5f,0.0f,
						-0.5f,0.5f,0.0f,
						0.5f,-0.5f,0.0f,

						-0.5f,0.5f,0.0f,
						0.5f,0.5f,0.0f,
						0.5f,-0.5f,0.0f
						};
GLfloat colors[] = {	0.0f, 0.0f, 0.0f, // 3d texture coords
						0.0f, 1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						};

// vertex array object
unsigned int vertexArrayObjID;
GLuint texID1, texID2, texID3, texID4;

// Reference to shader program
GLuint program;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("textured.vert", "textured.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for colour data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

	texID1 = LoadTexture("tga.tga", 1); // Flip-stödet är nu korrekt!
	texID2 = LoadTexture("png.png", 1); // Flipped.
	texID3 = LoadTexture("ppm.ppm", 1); // Laddar åt rätt håll!
	texID4 = LoadTexture("jpg.jpeg", 1); // Flipped. Klipper?! FIXAD!

// Defaults to unit 0
	glUniform1i(glGetUniformLocation(program,"texUnit"),0);

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO

	glBindTexture(GL_TEXTURE_2D, texID1);
	glUniform1f(glGetUniformLocation(program,"dx"),-0.5);
	glUniform1f(glGetUniformLocation(program,"dy"),0.5);
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
	
	glBindTexture(GL_TEXTURE_2D, texID2);
	glUniform1f(glGetUniformLocation(program,"dx"),0.5);
	glUniform1f(glGetUniformLocation(program,"dy"),0.5);
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
	
	glBindTexture(GL_TEXTURE_2D, texID3);
	glUniform1f(glGetUniformLocation(program,"dx"),-0.5);
	glUniform1f(glGetUniformLocation(program,"dy"),-0.5);
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
	
	glBindTexture(GL_TEXTURE_2D, texID4);
	glUniform1f(glGetUniformLocation(program,"dx"),0.5);
	glUniform1f(glGetUniformLocation(program,"dy"),-0.5);
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Multiple format texture loading example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}
