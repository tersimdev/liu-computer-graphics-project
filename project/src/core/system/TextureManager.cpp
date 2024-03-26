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
    textureStore.clear();
}

GLuint TextureManager::get_texture(std::string name)
{
    if (textureStore.find(name) == textureStore.end())
        return -1;
    return textureStore[name];
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
