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
    add_texture("ikea", "tga");
    add_texture("wall", "tga");
    add_texture("banana", "tga");

    add_texture("birchwood", "tga");
    add_texture("beanbag", "tga");
    add_texture("table_col", "tga");
    add_texture("table_rou", "tga");

    add_texture("spider2", "tga");
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
