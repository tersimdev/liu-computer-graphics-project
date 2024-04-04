// Project Main file

#define MAIN
#include "MicroGlut.h"
#include "SimpleGUI.h"
#include "util/Debug.h"
#include "util/Constants.h"
#include "util/Input.h"
#include "util/Camera.h"
#include "core/system/GraphicsManager.h"
#include "core/system/SceneManager.h"

double timeElapsed = 0;
unsigned int repeatingTimer = 16;
int currFPS = 0; 
Camera camera;
GraphicsManager graphicsMgr;
SceneManager sceneMgr;

float testf = 5; // test for simplegui

// runs once at the start
void init(void)
{
	dumpInfo();
	debug_log("[Initialising Systems]\n");
	Input::init();
	camera.init();
	graphicsMgr.init(&camera);
	sceneMgr.init(&graphicsMgr);
	debug_log("[Intialized Systems]\n");
}

// runs every tick
void display(void)
{
	double dt = timeElapsed; // set to previous
	timeElapsed = ((double)glutGet(GLUT_ELAPSED_TIME) / 1000.0); // divide by 1000f to convert to seconds
	dt = (timeElapsed - dt); 

	// debug_log("Tick, dt: %.4f\n", dt);
	if (dt > 0.01)
		currFPS = 1.0 / dt;
	sgCreateDisplayInt(10, 20, "FPS:", &currFPS);

	Input::update(dt);
	camera.update(dt);
	// camera.example_movement(dt);
	graphicsMgr.update(dt, timeElapsed);
	sceneMgr.update(dt);
}

void cleanup()
{
	debug_log("[Cleaning Up]\n");
	sceneMgr.cleanup();
	graphicsMgr.cleanup();
}

void passive_motion(int x, int y)
{
	Input::on_mouse_move(x, y);
}
void mouse(int button, int state, int x, int y)
{
	// todo send mouse click to input class as well

	if (button == 0)
		sgMouse(state, x, y);
}

void motion(int x, int y)
{
	sgMouseDrag(x, y);
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
	glutRepeatingTimer(repeatingTimer);
	glutPassiveMotionFunc(passive_motion);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	std::atexit(cleanup); // register exit cleanup

	glutMainLoop();

	exit(0); // doesnt reach here
}