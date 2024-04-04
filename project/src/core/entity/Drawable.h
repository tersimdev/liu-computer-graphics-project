#pragma once
#include "Material.h"
#include "LittleOBJLoader.h"

typedef struct _Transform
{
    vec3 translation, rotate, scale;
    //constructor for convenience
    _Transform(vec3 trl = {0,0,0}, vec3 rot = {0,0,0}, vec3 sca = {1,1,1})
        : translation(trl), rotate(rot), scale(sca) {}
} Transform;

class Drawable
{
public:
    Drawable()
    {
        transform = Transform();
        model = nullptr;
        material = nullptr;
        drawFlag = true;
    }

    void setTransform(Transform tr) { this->transform = tr; }
    Transform* getTransform() { return &transform; }

    void translate(vec3 t) { transform.translation += t; }
    void scale(vec3 s) { transform.scale = ElementMult(transform.scale, s); }
    void rotate(vec3 r) { transform.rotate += r; }

    Material *getMaterial() { return material; }
    Model *getModel() { return model; }
    void setMaterial(Material *m) { material = m; }
    void setModel(Model *m) { model = m; }

    bool getDrawFlag() { return drawFlag; }
    void setDrawFlag(bool d) { drawFlag = d; }

private:
    Transform transform;
    Model *model;
    Material *material;

    bool drawFlag; // flag of whether to draw
};