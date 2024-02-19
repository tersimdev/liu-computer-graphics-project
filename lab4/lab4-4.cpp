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
// terrain variables
vec3 terrainScale = {10, 100.0, 10};

GLfloat getHeight(TextureData *tex, unsigned int x, unsigned int z)
{
	// check for bounds, return -1 if invalid
	if (x < 0 || z < 0 || x >= tex->width || z >= tex->height)
		return -999;
	return (GLfloat)tex->imageData[(x + z * tex->width) * (tex->bpp / 8)];
}

Model *GenerateTerrain(TextureData *tex, vec3 scale, float tileFactor)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	unsigned int x, z;
	vec3 currVertex;
	vec3 neighbours[6];
	vec3 nbrNormals[6];

	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

	printf("bpp %d\n", tex->bpp);
	printf("w %d, h %d\n", tex->width, tex->height);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// get current vertex pos
			currVertex = {x / scale.x, getHeight(tex, x, z) / scale.y, z / scale.z};

			// init nbrs to 0
			for (int i = 0; i < 6; ++i)
			{
				neighbours[i] = currVertex;
				nbrNormals[i] = {0, 0, 0}; // init normals
			}

			bool hasLeft = (x > 0);
			bool hasRight = (x < tex->width - 1);
			bool hasUp = (z > 0);
			bool hasDown = (z < tex->height - 1);
			// get left right up down up-right down-left neighbours
			if (hasLeft)
				neighbours[0] = {(float)x - 1, -1, (float)z}; // left
			if (hasRight)
				neighbours[1] = {(float)x + 1, -1, (float)z}; // right
			if (hasUp)
				neighbours[2] = {(float)x, -1, (float)z - 1}; // up
			if (hasDown)
				neighbours[3] = {(float)x, -1, (float)z + 1}; // down
			if (hasUp && hasRight)
				neighbours[4] = {(float)x + 1, -1, (float)z - 1}; // up-right
			if (hasDown && hasLeft)
				neighbours[5] = {(float)x - 1, -1, (float)z + 1}; // down-left
			for (int i = 0; i < 6; ++i)
			{
				neighbours[i].y = getHeight(tex, neighbours[i].x, neighbours[i].z) / scale.y;
				neighbours[i].x /= scale.x;
				neighbours[i].z /= scale.z;
				// printf("nbr i %d: %f,%f,%f\n", i, neighbours[i].x, neighbours[i].y, neighbours[i].z);
			}
			// get neighbouring triangle normals
			if (hasUp) // has up
			{
				if (hasLeft)																		   // has left
					nbrNormals[0] = cross((neighbours[2] - currVertex), (neighbours[0] - currVertex)); // up x left
				if (hasRight)																		   // has right
				{
					nbrNormals[1] = cross((neighbours[4] - currVertex), (neighbours[2] - currVertex)); // up-right x up
					nbrNormals[2] = cross((neighbours[1] - currVertex), (neighbours[4] - currVertex)); // right x up-right
				}
			}
			if (hasDown) // has down
			{
				if (hasRight)																		   // has right
					nbrNormals[3] = cross((neighbours[3] - currVertex), (neighbours[1] - currVertex)); // down x right
				if (hasLeft)																		   // has left
				{
					nbrNormals[4] = cross((neighbours[5] - currVertex), (neighbours[3] - currVertex)); // down-left x down
					nbrNormals[5] = cross((neighbours[0] - currVertex), (neighbours[5] - currVertex)); // left x down-left
				}
			}

			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)] = currVertex;
			// Normal vectors. You need to calculate these.
			vec3 normal = {0, 0.01, 0};
			// sum up all triangle normals and do weighted sum
			for (int i = 0; i < 6; ++i)
			{
				normal += nbrNormals[i];
				// printf("i=%d, adding x,z: (%f,%f), n: %f %f %f\n", i, neighbours[i].x * scale.x, neighbours[i].z * scale.z, nbrNormals[i].x, nbrNormals[i].y, nbrNormals[i].z);
			}
			normal = normalize(normal);
			// printf("x,z: (%d,%d), n: %f %f %f\n", x, z, normal.x, normal.y, normal.z);
			normalArray[(x + z * tex->width)] = normal;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = (float)x / tex->width * tileFactor;
			texCoordArray[(x + z * tex->width)].y = (float)z / tex->height * tileFactor;
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
GLuint tex1, tex2;
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
// light variables
vec3 lightDir = vec3(0, -1, 0);
vec4 lightCol = vec4(0.8, 0.8, 0.7, 1);

// whether to draw wireframe
bool wireframe = false;

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
	LoadTGATextureSimple("conc.tga", &tex1);

	// lights
	glUniform3fv(glGetUniformLocation(program, "lightDir"), 1, &(lightDir.x));
	glUniform4fv(glGetUniformLocation(program, "lightCol"), 1, &(lightCol.r));

	// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex, terrainScale, 50);
	printError("init terrain");

	// printf("Note: The call to DrawModel will report warnings about inNormal not existing. This is because inNormal is not used in the shader yet so it is optimized away.\n");
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

	mat4 modelView, camMatrix;
	mat3 normal;

	printError("pre display");

	glUseProgram(program);
	// Build matrix

	camMatrix = lookAt(camPos, camPos + camDir, camUp);
	modelView = IdentityMatrix();
	normal = transpose(inverse(mat4tomat3(modelView)));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_TRUE, normal.m);

	glBindTexture(GL_TEXTURE_2D, tex1); // Bind Our Texture tex1
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// DrawWireframeModel(tm, program, "inPosition", "inNormal", "inTexCoord"); //kinda buggy
	}
	else
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

	if (glutKeyIsDown('1'))
	{
		wireframe = false;
	}
	if (glutKeyIsDown('2'))
	{
		wireframe = true;
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
How did you figure out what surface to use?

How did you calculate the height from the surface?

*/
