#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"

#define MAX_OBJS 100
#define MAx_TEXTURES 8

#define LABNUM "3-2"

//struct that combines TRS for ease of use
typedef struct _Transform {
    vec3 translation, rotate, scale;
} Transform;

//struct to define a object to be drawn in the scene
typedef struct _SceneObj {
    const char* name;
    Model* model;
    GLuint textures[MAx_TEXTURES];
    int currTexture; //texture to use, supports single texture only for now
    GLuint shaderProgram; //current shader
    Transform transform; //current translation, rotation, scale vectors

} SceneObj;

//Globals
// target update time in frames per second
unsigned int targetFPS = 60;
// total time elapsed
GLfloat t = 0;
// projection matrix
#define P_NEAR 1.0
#define P_FAR 30.0
#define P_RIGHT 0.5
#define P_LEFT -0.5
#define P_TOP 0.5
#define P_BOTTOM -0.5
mat4 proj;
// camera variables
vec3 camPos, camDir, camUp;
mat4 view;
//list of scene objects
SceneObj* sceneObjs[MAX_OBJS];
int sceneObjIdx = 0;
//current program
GLuint currProgram;

// helper funcs defined below
Transform CreateTransform(vec3 trans, vec3 rot, vec3 scale);
SceneObj* CreateSceneObj(const char* name, Model* model, const char* texturePath0, GLuint program, Transform transform);
SceneObj* CreateSceneObj(const char* name, const char* modelPath, const char* texturePath0, GLuint program, Transform transform);
void RemoveSceneObj(SceneObj* obj); //todo, free memory
SceneObj* CreateGroundPlane(float groundSize, float uvTiles, const char* groundTexture, GLuint program);
void DrawSceneObj(SceneObj* sceneObj);

void init(void)
{
    dumpInfo();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // load proj matrix
    proj = frustum(P_LEFT, P_RIGHT, P_BOTTOM, P_TOP, P_NEAR, P_FAR);
    // set up camera
    camPos = vec3(0, 2, 5);
    camDir = normalize((vec3(0) - camPos)); // look at origin
    camUp = vec3(0, 1, 0);

    // Load and compile shader(s)
    GLuint lit_shader = loadShaders("lab" LABNUM ".vert", "lab" LABNUM ".frag");
    printError("init shader");
    currProgram = lit_shader; //for now

    // Set static shader uniforms for each program
    glUniformMatrix4fv(glGetUniformLocation(lit_shader, "projectionMtx"), 1, GL_TRUE, proj.m);
    glUniform1i(glGetUniformLocation(lit_shader, "texUnit"), 0); // Texture unit 0
    printError("init uniforms");

    // Set display refresh, 1000ms / FPS
    glutRepeatingTimer(1000 / targetFPS);


    //create scene objects here
    Transform t;
    sceneObjIdx = 0;
    sceneObjs[sceneObjIdx++] = CreateGroundPlane(100.f, 20.f, "textures/grass.tga", lit_shader);
    t = CreateTransform(vec3(0,1,1), vec3(0), vec3(1));
    sceneObjs[sceneObjIdx++] = CreateSceneObj("bunny", "models/bunny.obj", "textures/grass.tga", lit_shader, t);
    t = CreateTransform(vec3(0,1,-2), vec3(0), vec3(0.2f));
    sceneObjs[sceneObjIdx++] = CreateSceneObj("teapot", "models/teapot.obj", "textures/conc.tga", lit_shader, t);
    printError("creating scene objects");

}

void display(void)
{
    // Get delta time dt in seconds
    GLfloat dt = t; // set to previous t
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    dt = (t - dt) / 1000; // convert to s
    // printf("DT: %.5f\n", dt); //track frame time

    // Calculate view matrix based on camera vectors
    view = lookAtv(camPos, camPos + camDir, camUp);

    // Set dynamic shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(currProgram, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(currProgram, "elapsedTime"), t / 1000.0f);

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw all scene objects
    for (int i = 0; i < MAX_OBJS; ++i) 
    {
        if (sceneObjs[i] == NULL)
            continue;
        
        DrawSceneObj(sceneObjs[i]);
    }

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab " LABNUM " (Terence and Peri)");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

/* Helper functions below */ //////////////////////////////

Transform CreateTransform(vec3 trans = {0,0,0}, vec3 rot = {0,0,0}, vec3 scale = {1,1,1}) 
{
    Transform t;
    t.translation = trans;
    t.rotate = rot;
    t.scale = scale;
    return t;
}
SceneObj* CreateSceneObj(const char* name, Model* model, const char* texturePath0, GLuint program, Transform transform)
{
    SceneObj* obj = (SceneObj*)malloc(sizeof(SceneObj));
    obj->name = name;
    obj->model = model;
    LoadTGATextureSimple(texturePath0, &(obj->textures[0]));
    obj->currTexture = 0; //as deafult
    obj->shaderProgram = program;
    obj->transform = transform;
    return obj;
}
SceneObj* CreateSceneObj(const char* name, const char* modelPath, const char* texturePath0, GLuint program, Transform transform)
{
    return CreateSceneObj(name, LoadModel(modelPath), texturePath0, program, transform);
}

SceneObj* CreateGroundPlane(float groundSize, float uvTiles, const char* groundTexture, GLuint program)
{
    vec3 vertices[] =
        {{-groundSize, 0.0f, -groundSize},
         {-groundSize, 0.0f, groundSize},
         {groundSize, -0.0f, -groundSize},
         {groundSize, -0.0f, groundSize}};
    vec3 normals[] =
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}};
    vec2 texCoords[] =
        {{0.0f, 0.0f},
         {0.0f, uvTiles},
         {uvTiles, 0.0f},
         {uvTiles, uvTiles}};
    GLuint indices[] = {0, 1, 2, 1, 3, 2};

    Model* m = LoadDataToModel(vertices, normals, texCoords, NULL, indices, 4,6);
    return CreateSceneObj("groundplane", m, groundTexture, program, CreateTransform());
}
void DrawSceneObj(SceneObj* sceneObj) 
{
    printf("Drawing %s\n", sceneObj->name);
    
    vec3 trans = sceneObj->transform.translation;
    vec3 scale = sceneObj->transform.scale;
    vec3 rot = sceneObj->transform.rotate;

    mat4 m_trans = T(trans.x, trans.y, trans.z);
    mat4 m_rot = Rx(rot.x) * Ry(rot.y) * Rz(rot.z); //order matters, consider quarternions for true representation
    mat4 m_scale = S(scale.x, scale.y, scale.z);
    mat4 m_model = m_trans * m_rot * m_scale;
    glBindTexture(GL_TEXTURE_2D, sceneObj->textures[sceneObj->currTexture]);                                             
    glActiveTexture(GL_TEXTURE0 + sceneObj->currTexture);
    glUniformMatrix4fv(glGetUniformLocation(sceneObj->shaderProgram, "modelMtx"), 1, GL_TRUE, m_model.m); 
    DrawModel(sceneObj->model, sceneObj->shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
}