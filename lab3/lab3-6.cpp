#include "GL_utilities.h"
#include "MicroGlut.h"
#include <math.h>
#include <stdio.h> // for printf

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"

#define RESX 600
#define RESY 600
#define MAX_OBJS 100
#define MAX_LIGHTS 8
#define MAx_TEXTURES 8

#define LABNUM "3-6"

// struct that combines TRS for ease of use
typedef struct _Transform
{
    vec3 translation, rotate, scale;
} Transform;

// struct to define a object to be drawn in the scene
typedef struct _SceneObj
{
    const char *name;
    Model *model;
    GLuint textures[MAx_TEXTURES];
    int currTexture;      // texture to use, supports single texture only for now
    GLuint shaderProgram; // current shader
    Transform transform;  // current translation, rotation, scale vectors
    float specularExponent; //temp, should be abstracted into a Material struct 

} SceneObj;

// struct to define a light source. only directional supported for now
typedef struct _LightSource
{
    int type;       // 0 for dir, 1 for point, 2 for spot
    vec4 color;     // alpha for intensity
    vec3 position;  // ignored for directional
    vec3 direction; // ignored for point
} LightSource;

// Globals
//  target update time in frames per second
unsigned int targetFPS = 60;
// total time elapsed
GLfloat t = 0;
// curr minus last mouse position
vec2 deltaMousePos = vec2(0, 0);
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
float camMoveSpeed = 2;
float camRotSpeed = 0.5f;
float mouseSens = 0.5f;
#define CAM_DEADZONE 0.1
#define MAX_CAM_LOOK_UP 0.8
mat4 view;
// list of scene objects
SceneObj *sceneObjs[MAX_OBJS];
int sceneObjIdx = 0;
// specific scene objs
SceneObj *skybox;
// shaders
GLuint lit_shader, unlit_shader;
// light sources
LightSource lights[MAX_LIGHTS];

// temp to make things spin
float phi = 0;

// helper funcs defined below
GLuint SetupShaders(const char *type);
void SetDynamicUniforms(void);
void HandleInput(GLfloat dt);
Transform CreateTransform(vec3 trans, vec3 rot, vec3 scale);
SceneObj *CreateSceneObj(const char *name, Model *model, const char *texturePath0, GLuint program, Transform transform);
SceneObj *CreateSceneObj(const char *name, const char *modelPath, const char *texturePath0, GLuint program, Transform transform);
void RemoveSceneObj(SceneObj *obj); // todo, free memory
SceneObj *CreateGroundPlane(float groundSize, float uvTiles, const char *groundTexture, GLuint program);
SceneObj *CreateSkyBox(float size, float height, GLuint program);
void DrawSceneObj(SceneObj *sceneObj);
void DrawSkyBox(SceneObj *sceneObj);

void init(void)
{
    dumpInfo();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE); //for skybox
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // load proj matrix
    proj = frustum(P_LEFT, P_RIGHT, P_BOTTOM, P_TOP, P_NEAR, P_FAR);
    // set up camera
    camPos = vec3(0, 1.5f, 1);
    camDir = normalize(vec3(0, -0.2, -1)); // look forward and a lil down
    camUp = vec3(0, 1, 0);

    // Load and compile shader(s)
    lit_shader = SetupShaders("lit");
    unlit_shader = SetupShaders("unlit");

    // Set display refresh, 1000ms / FPS
    glutRepeatingTimer(1000 / targetFPS);

    // create scene objects here
    Transform t;
    sceneObjIdx = 0;
    sceneObjs[sceneObjIdx++] = CreateGroundPlane(100.f, 100.f, "textures/grass.tga", lit_shader);
    skybox = CreateSkyBox(1.0f, -0.2f, unlit_shader);
    // spawn houses in one line
    for (int i = 0; i < 64 && sceneObjIdx < MAX_OBJS; ++i)
    {
        int x = i % 4;
        int z = i / 4;
        t = CreateTransform(vec3(-0.75f + 0.5f * x, 0.5f, -z - 2), vec3(0, -45, 0), vec3(0.4f));
        const char *textures[3] = {"textures/bricks.tga", "textures/conc.tga", "textures/bark2.tga"};
        int id = i % 3;
        sceneObjs[sceneObjIdx++] = CreateSceneObj("cow", "models/cow.obj", textures[id], lit_shader, t);
        sceneObjs[i]->specularExponent = i * 2;
    }
    printError("creating scene objects");

    // set up light sources here
    lights[0].color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    lights[0].direction = normalize(vec3(1, 0.5f, 0));
    printError("creating lights");
}

void display(void)
{
    // Get delta time dt in seconds
    GLfloat dt = t; // set to previous t
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    dt = (t - dt) / 1000.0f; // convert to s
    // printf("DT: %.5f\n", dt); //track frame time

    // Handle all mouse and keyboard input
    HandleInput(dt);
    // Move pointer back to middle
    glutWarpPointer(RESX / 2, RESY / 2);

    // spinnn
    phi += dt * 2;

    // Calculate view matrix based on camera vectors
    view = lookAtv(camPos, camPos + camDir, camUp);

    // Set dynamic shader uniforms
    SetDynamicUniforms();

    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw skybox
    DrawSkyBox(skybox);

    // draw all scene objects
    for (int i = 0; i < MAX_OBJS; ++i)
    {
        if (sceneObjs[i] == NULL)
            continue;

        // temp, to make them spin
        if (strcmp(sceneObjs[i]->name, "cow") == 0)
        {
            sceneObjs[i]->transform.rotate = vec3(0, phi + i * 3, 0);
        }

        DrawSceneObj(sceneObjs[i]);
    }

    printError("display");

    glutSwapBuffers();
}

void mouse_move(int x, int y)
{
    // calculate how far mouse moved from middle
    deltaMousePos.x = x - RESX / 2;
    deltaMousePos.y = y - RESY / 2;
    // printf("%d, %d\n", x, y);
    // printf("delta %f, %f\n", deltaMousePos.x, deltaMousePos.y);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(RESX, RESY);
    glutCreateWindow("Lab " LABNUM " (Terence and Peri)");
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouse_move);
    glutWarpPointer(RESX / 2, RESY / 2);
    init();
    glutMainLoop();
    return 0;
}

/* Helper functions below */ //////////////////////////////

GLuint SetupShaders(const char *type)
{
    char buf_v[128];
    char buf_f[128];
    snprintf(buf_v, 128, "lab" LABNUM "_%s.vert", type);
    snprintf(buf_f, 128, "lab" LABNUM "_%s.frag", type);
    GLuint ret = loadShaders(buf_v, buf_f);
    // Set common global static shader uniforms
    glUseProgram(ret);
    glUniformMatrix4fv(glGetUniformLocation(ret, "projectionMtx"), 1, GL_TRUE, proj.m);
    glUniform1i(glGetUniformLocation(ret, "texUnit"), 0); // Texture unit 0
    glUseProgram(0);
    return ret;
}
void SetDynamicUniforms(void)
{
    glUseProgram(lit_shader);
    glUniformMatrix4fv(glGetUniformLocation(lit_shader, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(lit_shader, "elapsedTime"), t / 1000.0f);
    //only one light for now
    glUniform3fv(glGetUniformLocation(lit_shader, "viewPos"), 1, &(camPos.x));
    glUniform3fv(glGetUniformLocation(lit_shader, "lightDir"), 1, &(lights[0].direction.x));
    glUniform4fv(glGetUniformLocation(lit_shader, "lightCol"), 1, &(lights[0].color.x));
    glUseProgram(unlit_shader);
    glUniformMatrix4fv(glGetUniformLocation(unlit_shader, "viewMtx"), 1, GL_TRUE, view.m);
    glUniform1f(glGetUniformLocation(unlit_shader, "elapsedTime"), t / 1000.0f);
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

Transform CreateTransform(vec3 trans = {0, 0, 0}, vec3 rot = {0, 0, 0}, vec3 scale = {1, 1, 1})
{
    Transform t;
    t.translation = trans;
    t.rotate = rot;
    t.scale = scale;
    return t;
}
SceneObj *CreateSceneObj(const char *name, Model *model, const char *texturePath0, GLuint program, Transform transform)
{
    SceneObj *obj = (SceneObj *)malloc(sizeof(SceneObj));
    obj->name = name;
    obj->model = model;
    LoadTGATextureSimple(texturePath0, &(obj->textures[0]));
    obj->currTexture = 0; // as deafult
    obj->shaderProgram = program;
    obj->transform = transform;
    obj->specularExponent = 64; //default
    return obj;
}
SceneObj *CreateSceneObj(const char *name, const char *modelPath, const char *texturePath0, GLuint program, Transform transform)
{
    return CreateSceneObj(name, LoadModel(modelPath), texturePath0, program, transform);
}

SceneObj *CreateGroundPlane(float groundSize, float uvTiles, const char *groundTexture, GLuint program)
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

    Model *m = LoadDataToModel(vertices, normals, texCoords, NULL, indices, 4, 6);
    SceneObj* ret = CreateSceneObj("groundplane", m, groundTexture, program, CreateTransform());
    ret->specularExponent = 0;
    return ret;
}
SceneObj *CreateSkyBox(float size, float height, GLuint program)
{
    SceneObj *ret = CreateSceneObj("skybox", "skybox/labskybox.obj", "skybox/labskybox512.tga", program, CreateTransform());
    ret->transform.translation.y = height;
    ret->transform.scale = vec3(size);
    return ret;
}

void DrawSceneObj(SceneObj *sceneObj)
{
    // printf("Drawing %s\n", sceneObj->name);

    vec3 trans = sceneObj->transform.translation;
    vec3 scale = sceneObj->transform.scale;
    vec3 rot = sceneObj->transform.rotate;

    mat4 m_trans = T(trans.x, trans.y, trans.z);
    mat4 m_rot = Rx(rot.x) * Ry(rot.y) * Rz(rot.z); // order matters, consider quarternions for true representation
    mat4 m_scale = S(scale.x, scale.y, scale.z);
    mat4 m_model = m_trans * m_rot * m_scale;
    mat3 m_normal = transpose(inverse(mat4tomat3(m_model)));
    glUseProgram(sceneObj->shaderProgram);
    glBindTexture(GL_TEXTURE_2D, sceneObj->textures[sceneObj->currTexture]);
    glActiveTexture(GL_TEXTURE0 + sceneObj->currTexture);
    glUniformMatrix4fv(glGetUniformLocation(sceneObj->shaderProgram, "modelMtx"), 1, GL_TRUE, m_model.m);
    glUniformMatrix3fv(glGetUniformLocation(sceneObj->shaderProgram, "normalMtx"), 1, GL_TRUE, m_normal.m);
    glUniform1f(glGetUniformLocation(sceneObj->shaderProgram, "specularExp"), sceneObj->specularExponent);
    DrawModel(sceneObj->model, sceneObj->shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
}

void DrawSkyBox(SceneObj *sceneObj)
{
    glDisable(GL_DEPTH_TEST);

    const Transform t = sceneObj->transform;
    mat4 m_trans = T(0, t.translation.y, 0);
    mat4 m_scale = S(t.scale.x, t.scale.y, t.scale.z);
    mat4 m_model = m_trans * m_scale;
    mat4 custom_view = mat3tomat4(mat4tomat3(view));
    glUseProgram(sceneObj->shaderProgram);
    glBindTexture(GL_TEXTURE_2D, sceneObj->textures[sceneObj->currTexture]);
    glActiveTexture(GL_TEXTURE0 + sceneObj->currTexture);
    glUniformMatrix4fv(glGetUniformLocation(sceneObj->shaderProgram, "modelMtx"), 1, GL_TRUE, m_model.m);
    glUniformMatrix4fv(glGetUniformLocation(sceneObj->shaderProgram, "viewMtx"), 1, GL_TRUE, custom_view.m);
    DrawModel(sceneObj->model, sceneObj->shaderProgram, "in_Position", "in_Normal", "in_TexCoord");

    glUniformMatrix4fv(glGetUniformLocation(sceneObj->shaderProgram, "viewMtx"), 1, GL_TRUE, view.m); // reset
    glEnable(GL_DEPTH_TEST);
}

/*
Questions (non-mandatory part):

Why do we get a white area in the distance for the specular light but not for the diffuse?

How do you generate a vector from the surface to the eye?

Which vectors need renormalization in the fragment shader?
*/