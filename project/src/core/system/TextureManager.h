#pragma once

#include <string>
#include <unordered_map>
#include "../entity/Material.h"
#include "../entity/ShaderProg.h"
/*
    Class to manage loaded textures for easy reuse
*/
class TextureManager
{
    public:
        void init();
        void cleanup();

        GLuint get_texture(std::string name); 
    private:
    std::unordered_map<std::string, GLuint> textureStore;
    
    void add_texture(std::string name, std::string ext, std::string path = ""); //note path is relative from TEXTURE_PATH
};