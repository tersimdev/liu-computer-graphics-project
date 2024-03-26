#include "TextureManager.h"

#include "LoadTGA.h" 
#include "../../util/Debug.h" 

void TextureManager::init()
{
    /*
        Add all textures to be used here!
    */
    add_texture("default", "tga");
    add_texture("conc", "tga");
}

void TextureManager::cleanup()
{
    for (auto [k,v] : materialStore)
        delete v;
    textureStore.clear();
    materialStore.clear();
}

GLuint TextureManager::get_texture(std::string name)
{
    if (textureStore.find(name) == textureStore.end())
        return -1;
    return textureStore[name];
}

Material *TextureManager::get_material(std::string name)
{
    if (materialStore.find(name) == materialStore.end())
        return nullptr;
    return materialStore[name];
}

Material *TextureManager::create_material(std::string name, ShaderProg shaderProg, vec3 albedo, vec4 specular)
{
    if (materialStore.find(name) != materialStore.end())
    {
        debug_error("Material with duplicate name found");
        return nullptr;
    }
    Material* m = new Material;
    m->albedo = albedo;
    m->specular = specular;
    m->shaderProg = shaderProg;
    m->textureBitmask = 0;
    materialStore[name] = m;
    return m;
}

Material *TextureManager::attach_texture_to_material(Material *m, int idx, GLuint texture)
{
    if (m == nullptr || idx < 0 || idx > MAX_TEXTURES || texture < 0)
    {
        debug_error("Invalid material/idx/texture.\n");
        return m;
    }
    
    m->textures[idx] = texture;
    m->textureBitmask = m->textureBitmask | (1 << idx);
    return m;
}

void TextureManager::add_texture(std::string name, std::string ext, std::string path)
{
    std::string fullpath = TEXTURES_PATH + path + name + "." + ext;
    if (ext == "tga")
    {
        GLuint tex;
        LoadTGATextureSimple(fullpath.c_str(), &tex);
        textureStore[name] = tex;
    }
}
