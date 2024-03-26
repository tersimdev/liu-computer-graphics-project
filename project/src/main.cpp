// Project Main file

#define MAIN
#include "MicroGlut.h"
#include "util/Debug.h"
#include "util/Constants.h"
#include "util/Input.h"
#include "util/Camera.h"
#include "core/system/ThingManager.h"
#include "core/system/GraphicsManager.h"
#include "core/system/SceneManager.h"
#include "core/system/TextureManager.h"

#include "LoadTGA.h"
#include "LittleOBJLoader.h"

float timeElapsed = 0;
unsigned int targetFPS = 60;
Input input;
Camera camera;
// ThingManager thingManager; //todo
GraphicsManager graphicsMgr;
SceneManager sceneMgr;
TextureManager textureMgr;

// runs once at the start
void init(void)
{
	dumpInfo();
	debug_log("[Initialising Systems]\n");
	input.init();
	camera.init(&input);
	graphicsMgr.init(&camera);
	textureMgr.init();
	sceneMgr.init(&graphicsMgr, &textureMgr);
	debug_log("[Intialized Systems]\n");
}

// runs every tick
void display(void)
{
	float dt = timeElapsed; // set to previous
	timeElapsed = (float)glutGet(GLUT_ELAPSED_TIME);
	dt = (timeElapsed - dt) / 1000.0f; // divide by 1000f to convert to seconds

	// debug_log("Tick, dt: %.4f\n", dt);

	input.update(dt);
	camera.update(dt);
	graphicsMgr.update(dt, timeElapsed);
	sceneMgr.update(dt);
}

void cleanup()
{
	debug_log("[Cleaning Up]\n");
	graphicsMgr.cleanup();
	sceneMgr.cleanup();
	textureMgr.cleanup();
}

void passive_motion(int x, int y)
{
	input.on_mouse_move(x, y);
}

void glutMainLoop(); // forward declare for intellisense
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(RES_X, RES_Y);
	glutCreateWindow(GAME_TITLE " - " AUTHOR);
	init();

	glutDisplayFunc(display);
	glutRepeatingTimer(1000 / targetFPS);
	glutPassiveMotionFunc(passive_motion);
	std::atexit(cleanup); // register exit cleanup

	glutMainLoop();

	exit(0); // doesnt reach here
}