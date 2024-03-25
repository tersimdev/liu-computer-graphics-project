// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

mat4 projectionMatrix;

Model *GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	unsigned int x, z;

	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 100.0;
			vertexArray[(x + z * tex->width)].z = z / 1.0;
			// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)].x = 0.0;
			normalArray[(x + z * tex->width)].y = 1.0;
			normalArray[(x + z * tex->width)].z = 0.0;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)].y = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width - 1; x++)
		for (z = 0; z < tex->height - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width - 1)) * 6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 1] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 2] = x + 1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width - 1)) * 6 + 3] = x + 1 + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 4] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model *model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);

	return model;
}

// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex0, tex1;
TextureData ttex; // terrain
// total time elapsed
GLfloat t = 0;
// curr minus last mouse position
vec2 deltaMousePos = vec2(0, 0);
// camera variables
vec3 camPos, camDir, camUp;
float camMoveSpeed = 2;
float camRotSpeed = 0.5f;
float mouseSens = 0.5f;
#define CAM_DEADZONE 0.1
#define MAX_CAM_LOOK_UP 0.8
#define RESX 600
#define RESY 600
//  target update time in frames per second
unsigned int targetFPS = 60;

// helper funcs
void HandleInput(GLfloat dt);

void init(void)
{
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	camPos = vec3(0, 5, 8);
	camDir = normalize(vec3(2, 0, 2) - camPos); // look forward and a lil down
	camUp = vec3(0, 1, 0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex0);

	// Load terrain data

	LoadTGATextureData("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	printf("Note: The call to DrawModel will report warnings about inNormal not existing. This is because inNormal is not used in the shader yet so it is optimized away.\n");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat dt = t; // set to previous t
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	dt = (t - dt) / 1000.0f; // convert to s
	HandleInput(dt);
	// Move pointer back to middle
	glutWarpPointer(RESX / 2, RESY / 2);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	glUseProgram(program);

	// Build matrix

	camMatrix = lookAt(camPos, camPos + camDir, camUp);
	modelView = IdentityMatrix();
	total = camMatrix * modelView;
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	
	glBindTexture(GL_TEXTURE_2D, tex0); // Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}

void mouse(int x, int y)
{
	// This function is included in case you want some hints about using passive mouse movement.
	// Uncomment to see mouse coordinates:
	// printf("%d %d\n", x, y);
	deltaMousePos.x = x - RESX / 2;
	deltaMousePos.y = y - RESY / 2;
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(600, 600);
	glutCreateWindow("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();
	glutRepeatingTimer(1000 / targetFPS);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}

void HandleInput(GLfloat dt)
{
	// calclulate some stuff i prob need
	vec3 camFront = camDir;
	camFront.y = 0;
	vec3 camRight = normalize(cross(camDir, camUp));
	camRight.y = 0; // incase

	// WASD movement, E up Q down
	if (glutKeyIsDown('w'))
	{
		// move along the front direction (view igrnoring y)
		camPos += camFront * camMoveSpeed * dt;
	}
	else if (glutKeyIsDown('s'))
	{
		// same as w but -1
		camPos += -camFront * camMoveSpeed * dt;
	}
	if (glutKeyIsDown('d'))
	{
		// calculate right and move along it
		camPos += camRight * camMoveSpeed * dt;
	}
	else if (glutKeyIsDown('a'))
	{
		// same as d but -1
		camPos += -camRight * camMoveSpeed * dt;
	}
	if (glutKeyIsDown('e'))
	{
		// move u in y
		vec3 up = vec3(0, 1, 0); // incase camUp is invalid
		camPos += up * camMoveSpeed * dt;
	}
	else if (glutKeyIsDown('q'))
	{
		// same as up but -1
		vec3 up = vec3(0, 1, 0); // incase camUp is invalid
		camPos += -up * camMoveSpeed * dt;
	}

	// Camera mouse movement
	if (deltaMousePos.x > CAM_DEADZONE || deltaMousePos.x < -CAM_DEADZONE)
	{
		camDir = ArbRotate(camUp, -deltaMousePos.x * dt * camRotSpeed * mouseSens) * camDir;
		camDir = normalize(camDir);
	}
	if ((deltaMousePos.y > CAM_DEADZONE && camDir.y > -MAX_CAM_LOOK_UP) ||
		(deltaMousePos.y < -CAM_DEADZONE && camDir.y < MAX_CAM_LOOK_UP))
	{
		camDir = ArbRotate(camRight, -deltaMousePos.y * dt * camRotSpeed * mouseSens) * camDir;
		camDir = normalize(camDir);
	}

	// keyboard controls for camera direction ijkl
	if (glutKeyIsDown('j'))
	{
		camDir = ArbRotate(camUp, dt * camRotSpeed) * camDir;
		camDir = normalize(camDir);
	}
	else if (glutKeyIsDown('l'))
	{
		camDir = ArbRotate(camUp, -dt * camRotSpeed) * camDir;
		camDir = normalize(camDir);
	}
	if (glutKeyIsDown('i') && camDir.y < MAX_CAM_LOOK_UP) // limit how far the cam can look up
	{
		camDir = ArbRotate(camRight, dt * camRotSpeed) * camDir;
		camDir = normalize(camDir);
	}
	else if (glutKeyIsDown('k') && camDir.y > -MAX_CAM_LOOK_UP) // limit how far the cam can look down
	{
		camDir = ArbRotate(camRight, -dt * camRotSpeed) * camDir;
		camDir = normalize(camDir);
	}

	// Exit button
	if (glutKeyIsDown(0x1B) || glutKeyIsDown('0'))
	{ // VK_ESCAPE or 0
		printf("Exit button pressed!\n");
		glutClose();
	}
}

/**
Questions:

Did you need to extend your camera movement code? How?
no

*/
