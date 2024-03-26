#pragma once

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils4.h"
#include "../entity/Drawable.h"
#include "../entity/Light.h"
#include "../entity/ShaderProg.h"
#include "../../util/Constants.h"
#include "../../util/Camera.h"
#include "TextureManager.h"

#include <unordered_map>
#include <string>
#include <vector>

class GraphicsManager
{
    public:
    void init(Camera* camera);
    void update(float dt, float elapsed);
    void cleanup();

    void set_drawable_list(std::vector<Drawable*>* list);
    void set_dir_light(Light* light);
    void set_point_light(int idx, Light* light);

    Camera* get_camera();
    GLuint get_shader(ShaderProg shaderProg);
private:
    std::unordered_map<ShaderProg, GLuint> shaderMap; //map of compiled shaders programs
    TextureManager textureMgr;
    // reference to a list of drawables to draw
    std::vector<Drawable*>* drawableList;
    // an array to store references to point lights in the scene
    Light* pointLights[MAX_POINT_LIGHTS];
    // reference to the directional light, intially null
    Light* dirLight;

    mat4 proj, view;
    Camera* camera; //reference to camera obj
    vec3 viewPos; //same as camPos, stored for uniforms
    float fovDeg = INITIAL_FOV; //fov of perspective

    //helper to load shader, wrapper around loadShaders from glUtils
    GLuint load_shaders(std::string vertex, std::string fragment);
    //sets uniforms that are constant for all objects using same shader
    void set_shader_uniforms(float elapsed);

    //sets material uniforms and draws a Drawable
    void render(Drawable* d);
};
