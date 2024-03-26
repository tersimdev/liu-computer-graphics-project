#include "DrawableHelper.h"
#include "../../util/Debug.h"

Material *DrawableHelper::create_material(std::string name, ShaderProg shaderProg, vec3 albedo, vec4 specular)
{
    Material *m = new Material;
    m->albedo = albedo;
    m->specular = specular;
    m->shaderProg = shaderProg;
    m->textureBitmask = 0;
    return m;
}

Material *DrawableHelper::attach_texture_to_material(Material *m, int idx, std::string textureName)
{
    if (m == nullptr || idx < 0 || idx > MAX_TEXTURES || textureName != "")
    {
        debug_error("Invalid material/idx/texture.\n");
        return m;
    }

    m->textures[idx] = textureName;
    m->textureBitmask = m->textureBitmask | (1 << idx);
    return m;
}

Drawable *DrawableHelper::create_from_model(std::string path, Transform t)
{
    Drawable *ret = new Drawable;
    ret->setModel(LoadModel((MODELS_PATH + path).c_str()));
    ret->setTransform(t);
    return ret;
}

Drawable *DrawableHelper::create_sphere(Transform t)
{
    // todo make a nice sphere using code
    return nullptr;
}

Drawable *DrawableHelper::create_cube(Transform t)
{
    // todo bring cube code from lab here
    return nullptr;
}

Drawable *DrawableHelper::create_plane(Transform t)
{
    // todo bring ground code from lab here
    return nullptr;
}
