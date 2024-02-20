// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// uncomment to use bigger terrain file
#define FFT

// helper to print every x frame
int currFrame = 0;
// macro to check currframe before printing
#define LOG_PRINTF(...) \
	if (currFrame == 0) \
	printf(__VA_ARGS__)

mat4 projectionMatrix;
// terrain variables
#ifdef FFT
vec3 terrainStep = {10, 100.0, 10};
#else
vec3 terrainStep = {0.5, 50.0, 0.5};
#endif

GLfloat getHeight(TextureData *tex, unsigned int x, unsigned int z)
{
	// check for bounds, return -1 if invalid
	if (x < 0 || z < 0 || x >= tex->width || z >= tex->height)
	{
		// printf("ERROR: tried to get texture data at x=%d z=%d\n", x, z);
		return -999;
	}
	return (GLfloat)tex->imageData[(x + z * tex->width) * (tex->bpp / 8)];
}

Model *GenerateTerrain(TextureData *tex, vec3 step, float tileFactor)
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
			currVertex = {x / step.x, getHeight(tex, x, z) / step.y, z / step.z};

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
				neighbours[i].y = getHeight(tex, neighbours[i].x, neighbours[i].z) / step.y;
				neighbours[i].x /= step.x;
				neighbours[i].z /= step.z;
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

GLfloat getObjectHeightFromTerrain(TextureData *tex, float worldX, float worldZ)
{
	vec3 scaledPos = {worldX * terrainStep.x, 0, worldZ * terrainStep.z};
	// LOG_PRINTF("1. scaledPos: x %f, z %f\n", scaledPos.x, scaledPos.z);
	if (scaledPos.x < 0 || scaledPos.x >= tex->width || scaledPos.z < 0 || scaledPos.z >= tex->height)
		return -1;

	vec3 topLeft, topRight, botLeft, botRight;
	topLeft.y = topRight.y = botLeft.y = botRight.y = 0;

	// find the quad
	topLeft.x = (unsigned int)(floorf(scaledPos.x));
	topLeft.z = (unsigned int)(floorf(scaledPos.z));

	// LOG_PRINTF("2. topLeft: x %f, z %f\n", topLeft.x, topLeft.z);

	topRight.x = topLeft.x + 1;
	topRight.z = topLeft.z;

	botLeft.x = topLeft.x;
	botLeft.z = topLeft.z + 1;

	botRight.x = topLeft.x + 1;
	botRight.z = topLeft.z + 1;

	// LOG_PRINTF("3. botRight: x %f, z %f\n", botRight.x, botRight.z);

	// use vector math to find out which
	bool inTopLeftTri = true;
	// use order of cross product to check which side of diagonal point is on
	// if point is on diagonal, cross will be zero, default to top left triangle
	vec3 diag = (botLeft - topRight);
	vec3 vecToPos = (scaledPos - topRight);
	diag.y = 0;
	vecToPos.y = 0;
	vec3 crossed = cross(diag, vecToPos);
	if (crossed.y > 0)
		inTopLeftTri = false;

	// LOG_PRINTF("4. Is topLeft triangle?: %d, cross: %f, %f, %f\n", (inTopLeftTri ? 1 : 0), crossed.x, crossed.y, crossed.z);

	GLfloat ret = 0;
	GLfloat h1, h2, h3;
	float d1, d2, d3;
	float totalArea;
	// use area of three sub triangle to calculate ratio
	if (inTopLeftTri)
	{
		h1 = getHeight(tex, topLeft.x, topLeft.z);
		totalArea = Norm(cross(diag, (topLeft - topRight)));
		d1 = Norm(cross((topRight - scaledPos), (botLeft - scaledPos))) / totalArea;
		d2 = Norm(cross((topLeft - scaledPos), (topRight - scaledPos))) / totalArea;
		d3 = Norm(cross((topLeft - scaledPos), (botLeft - scaledPos))) / totalArea;
	}
	else
	{
		h1 = getHeight(tex, botRight.x, botRight.z);
		totalArea = Norm(cross(diag, (botRight - topRight)));
		d1 = Norm(cross((topRight - scaledPos), (botLeft - scaledPos))) / totalArea;
		d2 = Norm(cross((botRight - scaledPos), (topRight - scaledPos))) / totalArea;
		d3 = Norm(cross((botLeft - scaledPos), (botRight - scaledPos))) / totalArea;
	}
	h2 = getHeight(tex, botLeft.x, botLeft.z);
	h3 = getHeight(tex, topRight.x, topRight.z);
	ret = 0;
	// interpolate the height
	if (h1 > 0)
		ret += h1 * d1;
	if (h2 > 0)
		ret += h2 * d2;
	if (h3 > 0)
		ret += h3 * d3;
	// divide by step
	ret /= terrainStep.y;
	// LOG_PRINTF("5. h 1,2,3: %f, %f, %f | d 1,2,3: %f, %f, %f\n", h1, h2, h3, d1, d2, d3);
	return ret;
}

// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex0, tex1, texball;
TextureData ttex; // terrain
// total time elapsed
GLfloat t = 0;
// curr minus last mouse position
vec2 deltaMousePos = vec2(0, 0);
// camera variables
int camMode = 0; // 0 for normal, 1 for ball
vec3 camPos, camDir, camUp;
float camMoveSpeed = 2;
float camRotSpeed = 0.5f;
float mouseSens = 0.5f;
#define CAM_DEADZONE 0.1
#define MAX_CAM_LOOK_UP 0.95
#define RESX 600
#define RESY 600
//  target update time in frames per second
unsigned int targetFPS = 60;
// light variables
vec3 lightDir = vec3(-0.5, -1, 0);
vec4 lightCol = vec4(0.7, 0.7, 0.5, 1.3);

// whether to draw wireframe
bool wireframe = false;

// variable to move octagon, ball
vec3 ballPos = vec3(0, 0, 0);
vec3 octagonPos = vec3(0, 0, 0);

// helper funcs
void HandleInput(GLfloat dt);
void InitCamera();
void MoveCamera(vec3 camFront, vec3 camRight, float dt);
void RotateCamera(vec3 camRight, float dt);

void init(void)
{
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	InitCamera();

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain-multi.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex0"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex1"), 1); // Texture unit 0
	LoadTGATextureSimple("conc.tga", &tex0);
	LoadTGATextureSimple("grass.tga", &tex1);

	// lights
	glUniform3fv(glGetUniformLocation(program, "lightDir"), 1, &(lightDir.x));
	glUniform4fv(glGetUniformLocation(program, "lightCol"), 1, &(lightCol.r));

	// Load terrain data

#ifdef FFT
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex, terrainStep, 100);
#else
	LoadTGATextureData("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex, terrainStep, 1);
#endif

	LoadTGATextureSimple("bricks.tga", &texball);
	// load ball on camera
	m = LoadModel("groundsphere.obj");
	m2 = LoadModel("octagon.obj");

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

	currFrame = (currFrame + 1) % 60;

	mat4 modelMatrix, camMatrix;
	mat3 normal;

	printError("pre display");

	// TEMP FOR LAB4-5
	// move octagon in circle
	octagonPos.x = 0.8 * (ttex.width / terrainStep.x) * (sinf(t / 10000.0f) * 0.5 + 0.5);
	octagonPos.z = 0.8 * (ttex.height / terrainStep.z) * (cosf(t / 10000.0f) * 0.5 + 0.5);

	glUseProgram(program);
	// Build matrix

	camMatrix = lookAt(camPos, camPos + camDir, camUp);
	modelMatrix = IdentityMatrix();
	normal = transpose(inverse(mat4tomat3(modelMatrix)));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_TRUE, normal.m);

	glUniform1i(glGetUniformLocation(program, "multiTexture"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0); // Bind Our Texture tex1
	glActiveTexture(GL_TEXTURE1);
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

	glUniform1i(glGetUniformLocation(program, "multiTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texball); // Bind for ball and octagon
	// draw ball
	float height = getObjectHeightFromTerrain(&ttex, ballPos.x, ballPos.z);
	// LOG_PRINTF(">>> BALL terrain height at (%f,%f) is %f\n", ballPos.x, ballPos.z, height);
	modelMatrix = T(ballPos.x, height, ballPos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelMatrix.m);
	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
	// draw octagon
	height = getObjectHeightFromTerrain(&ttex, octagonPos.x, octagonPos.z);
	LOG_PRINTF(">>> OCTAGON terrain height at (%f,-,%f) is %f\n", octagonPos.x, octagonPos.z, height);
	modelMatrix = T(octagonPos.x, height, octagonPos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelMatrix.m);
	DrawModel(m2, program, "inPosition", "inNormal", "inTexCoord");

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
	vec3 camFront, camRight;
	if (camMode == 0)
	{
		camFront = camDir;
		camFront.y = 0;
		camRight = normalize(cross(camDir, camUp));
		camRight.y = 0; // incase
		RotateCamera(camRight, dt);
		MoveCamera(camFront, camRight, dt);
	}

	// TEMP FOR LAB4-5
	{
		// move ball in x and z axis
		if (glutKeyIsDown('j'))
			ballPos.x -= camMoveSpeed * dt;
		else if (glutKeyIsDown('l'))
			ballPos.x += camMoveSpeed * dt;

		if (glutKeyIsDown('i'))
			ballPos.z -= camMoveSpeed * dt;

		else if (glutKeyIsDown('k'))
			ballPos.z += camMoveSpeed * dt;
	}

	// special toggles below
	if (glutKeyIsDown('1'))
	{
		wireframe = false;
	}
	if (glutKeyIsDown('2'))
	{
		wireframe = true;
	}

	if (glutKeyIsDown('3'))
	{
		camMode = 0;
		InitCamera();
	}

	// Exit button
	if (glutKeyIsDown(0x1B) || glutKeyIsDown('0'))
	{ // VK_ESCAPE or 0
		printf("Exit button pressed!\n");
		glutClose();
	}
}

void InitCamera()
{
	if (camMode == 0)
	{
		camPos = vec3(0, 5, 10);
		camDir = normalize(vec3(0, 0, 0) - camPos); // look at origin
		camUp = vec3(0, 1, 0);
	}
}
void MoveCamera(vec3 camFront, vec3 camRight, float dt)
{
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
}
void RotateCamera(vec3 camRight, float dt)
{
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

	// uncomment if needed
	// keyboard controls for camera direction ijkl
	{
		// if (glutKeyIsDown('j'))
		// {
		// 	camDir = ArbRotate(camUp, dt * camRotSpeed) * camDir;
		// 	camDir = normalize(camDir);
		// }
		// else if (glutKeyIsDown('l'))
		// {
		// 	camDir = ArbRotate(camUp, -dt * camRotSpeed) * camDir;
		// 	camDir = normalize(camDir);
		// }
		// if (glutKeyIsDown('i') && camDir.y < MAX_CAM_LOOK_UP) // limit how far the cam can look up
		// {
		// 	camDir = ArbRotate(camRight, dt * camRotSpeed) * camDir;
		// 	camDir = normalize(camDir);
		// }
		// else if (glutKeyIsDown('k') && camDir.y > -MAX_CAM_LOOK_UP) // limit how far the cam can look down
		// {
		// 	camDir = ArbRotate(camRight, -dt * camRotSpeed) * camDir;
		// 	camDir = normalize(camDir);
		// }
	}
}

/**
Questions:
What kind of multitexturing function did you implement?
*/
