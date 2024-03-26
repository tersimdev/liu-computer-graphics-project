#include "DrawableHelper.h"

Drawable *DrawableHelper::create_from_model(std::string path, Transform t)
{
    Drawable* ret = new Drawable;
    ret->setModel(LoadModel((MODELS_PATH + path).c_str()));
    ret->setTransform(t);
    return ret;
}

Drawable *DrawableHelper::create_sphere(Transform t)
{
    //todo make a nice sphere using code
    return nullptr;
}

Drawable *DrawableHelper::create_cube(Transform t)
{
    //todo bring cube code from lab here
    return nullptr;
}

Drawable *DrawableHelper::create_plane(Transform t)
{
    //todo bring ground code from lab here
    return nullptr;
}
