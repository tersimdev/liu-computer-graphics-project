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

#include "LoadTGA.h"
#include "LittleOBJLoader.h"

float timeElapsed = 0;
unsigned int targetFPS = 60;
Input input;
Camera camera;
// ThingManager thingManager; //todo
GraphicsManager graphicsMgr;
SceneManager sceneMgr;

// runs once at the start
void init(void)
{
	dumpInfo();
	debug_log("[Initialising Systems]\n");
	input.init();
	camera.init(&input);
	graphicsMgr.init(&camera);
	sceneMgr.init();
	debug_log("[Intialized Systems]\n");

	// temp
	Drawable *d = new Drawable;
	Material *m = new Material;
	LoadTGATextureSimple("asset/texture/conc.tga", &(m->textures[0]));
	m->textureBitmask = 1;
	m->shaderProg = graphicsMgr.get_shader(ShaderProg::LIT);
	// m->textureBitmask = 0;
	m->albedo = {0.7, 0.2, 0.2}; // red sphere
	// m->specular = { 0, 0, 0 ,0};
	m->specular = {0.8, 0.8, 0.8, 16};
	// d->setModel(LoadModel("asset/model/groundsphere.obj"));
	d->setModel(LoadModel("asset/model/bunnyplus.obj"));
	d->setMaterial(m);
	d->translate({0, 0, -5});
	d->scale({0.5, 0.5, 0.5});
	graphicsMgr.add_obj(d);
	Light *dlt = new Light;
	dlt->color = vec4(0.8, 0.8, 0.6, 1);
	dlt->position = vec3(-1, -1, -0.5);
	graphicsMgr.set_dir_light(dlt);
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