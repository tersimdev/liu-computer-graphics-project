#pragma once

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils4.h"
#include "../Drawable.h"
#include "../Light.h"
#include "../../util/Constants.h"
#include "../Camera.h"

#include <unordered_map>
#include <string>
#include <vector>

enum ShaderProg
{
    LIT,
    UNLIT,
    NUM_PROGS
};

class GraphicsManager
{
    public:
    void init(Camera* camera);
    void update(float dt, float elapsed);
    void cleanup();

    void add_obj(Drawable* drawable);
    void remove_obj(Drawable* drawable);

    void set_dir_light(Light* light);
    void set_point_light(int idx, Light* light);

    GLuint get_shader(ShaderProg shaderProg);
private:
    std::vector<Drawable*> objArray; //list of drawables
    std::unordered_map<ShaderProg, GLuint> shaderMap; //map of compiled shaders programs

    mat4 proj, view;
    Camera* camera; //reference to camera obj
    vec3 viewPos; //same as camPos, stored for uniforms

    //an array to store references to point lights in the scene
    Light* pointLights[MAX_POINT_LIGHTS];
    //reference to the directional light, intially null
    Light* dirLight;

    //helper to load shader, wrapper around loadShaders from glUtils
    GLuint load_shaders(std::string vertex, std::string fragment);
    //sets uniforms that are constant for all objects using same shader
    void set_shader_uniforms(float elapsed);

    //sets material uniforms and draws a Drawable
    void render(Drawable* d);
};
