#include "GraphicsManager.h"
#include "SimpleGUI.h"

void GraphicsManager::init(Camera *camera)
{
    this->camera = camera;
    textureMgr.init();

    // GL inits
    glClearColor(CLEAR_COL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("GL inits");

    // load proj matrix using fov
    proj = perspective(fovDeg, (float)RES_X / RES_Y, P_Z_NEAR, P_Z_FAR);

    // Load and compile shader(s)
    shaderMap[LIT] = load_shaders("lit.vert", "lit.frag");
    shaderMap[UNLIT] = load_shaders("unlit.vert", "unlit.frag");
    shaderMap[SIMPLE] = load_shaders("unlit.vert", "simple.frag");
}

void GraphicsManager::update(float dt, float elapsed)
{
    if (camera == nullptr)
    {
        debug_error("Camera reference is null!\n");
        return;
    }
    if (drawableList == nullptr)
    {
        // debug_log("No drawable list yet\n");
        return;
    }

    viewPos = camera->get_pos();
    // Calculate view matrix based on camera vectors
    view = lookAtv(viewPos, camera->get_lookat(), camera->get_up());
    // Set uniforms constant for shader for all objects
    set_shader_uniforms(elapsed);
    printError("pre display");

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw available drawables
    // TODO sort drawables by distance, and transperency if needed
    for (Drawable *d : *drawableList)
        render(d);

    printError("display");
    //draw gui
    sgDraw();
    // Swap buffers
    glutSwapBuffers();
}

void GraphicsManager::cleanup()
{
    textureMgr.cleanup();
}

void GraphicsManager::set_drawable_list(std::vector<Drawable *> *list)
{
    drawableList = list;
}

void GraphicsManager::set_dir_light(Light *light)
{
    if (!light)
        return;
    light->position = normalize(light->position);
    dirLight = light;
}

void GraphicsManager::set_point_light(int idx, Light *light)
{
    if (!light)
        return;
    if (idx >= 0 && idx < MAX_POINT_LIGHTS)
        pointLights[idx] = light;
}

Camera *GraphicsManager::get_camera()
{
    return camera;
}

GLuint GraphicsManager::get_shader(ShaderProg shaderProg)
{
    if (shaderMap.find(shaderProg) == shaderMap.end())
        return -1;
    return shaderMap[shaderProg];
}

GLuint GraphicsManager::load_shaders(std::string vertex, std::string fragment)
{
    GLuint ret = loadShaders((SHADER_PATH + vertex).c_str(), (SHADER_PATH + fragment).c_str());
    // Set common global static shader uniforms that do not change throughout program
    glUseProgram(ret);
    glUniformMatrix4fv(glGetUniformLocation(ret, "projectionMtx"), 1, GL_TRUE, proj.m);
    glUseProgram(0);
    return ret;
}

void GraphicsManager::set_shader_uniforms(float elapsed)
{
    GLuint prog;
    for (auto &[key, value] : shaderMap)
    {
        prog = value;
        glUseProgram(prog);
        glUniformMatrix4fv(glGetUniformLocation(prog, "viewMtx"), 1, GL_TRUE, view.m);
        glUniform1f(glGetUniformLocation(prog, "elapsedTime"), elapsed / 1000.0f);

        // based on name, set extra uniforms
        switch (key)
        {
        case LIT:
        {
            glUniform3fv(glGetUniformLocation(prog, "viewPos"), 1, &(viewPos.x));
            if (dirLight != nullptr)
            {
                glUniform3fv(glGetUniformLocation(prog, "lightDir"), 1, &(dirLight->position.x));
                glUniform4fv(glGetUniformLocation(prog, "lightCol"), 1, &(dirLight->color.x));
            }
            break;
        }
        case UNLIT:
        {
            break;
        }
        default:
            break;
        }
    }
}

void GraphicsManager::render(Drawable *d)
{
    if (d == nullptr || !d->getDrawFlag())
        return;

    vec3 trans = d->getTransform().translation;
    vec3 scale = d->getTransform().scale;
    vec3 rot = d->getTransform().rotate;

    // mat4 m_trans = T(trans.x, trans.y, trans.z);
    // mat4 m_rot = Rx(rot.x) * Ry(rot.y) * Rz(rot.z); // order matters, consider quarternions for true representation
    // mat4 m_scale = S(scale.x, scale.y, scale.z);
    // mat4 m_model = m_trans * m_rot * m_scale;
    // Calculate TRS in one line to save memory
    mat4 m_model = T(trans.x, trans.y, trans.z) * (Rx(rot.x) * Ry(rot.y) * Rz(rot.z)) * S(scale.x, scale.y, scale.z);
    mat3 m_normal = transpose(inverse(mat4tomat3(m_model)));

    Material *m = d->getMaterial();
    if (m == nullptr)
    {
        debug_error("Material is null (Drawable: %p)\n", d);
        return;
    }
    GLuint program = get_shader(m->shaderProg);
    if (program < 0)
    {
        debug_error("Shader program is invalid\n");
        return;
    }

    glUseProgram(program);
    // set model matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMtx"), 1, GL_TRUE, m_model.m);
    glUniformMatrix3fv(glGetUniformLocation(program, "normalMtx"), 1, GL_TRUE, m_normal.m);

    // set material uniforms
    for (int i = 0; i < MAX_TEXTURES; ++i)
    {
        // if bitmask is 1
        if ((m->textureBitmask >> i) & 1)
        {
            int texUnit = textureMgr.get_texture(m->textures[i]);
            if (texUnit < 0)
                continue;
            glBindTexture(GL_TEXTURE_2D, texUnit);
            glActiveTexture(GL_TEXTURE0 + i);
        }
    }
    glUniform3fv(glGetUniformLocation(program, "albedo"), 1, &(m->albedo.r));
    glUniform4fv(glGetUniformLocation(program, "specular"), 1, &(m->specular.r));

    DrawModel(d->getModel(), program, "in_Position", "in_Normal", "in_TexCoord");
}
