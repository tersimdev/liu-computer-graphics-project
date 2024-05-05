#include "Floor.h"

void Floor::init()
{
    // defaults
    vec3 pos = {0,0,0};
    vec2 scale = {10.0f, 10.0f};
    this->drawable = dh::create_from_model("plane.obj", Transform(pos, {0, 0, 0}, {scale.x, 1, scale.y}));
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0, 0, 0, 0});
    m->tileFactor = 20;
    dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);
    this->collider = new PlaneCollider(pos, {0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {scale.x, scale.y});
    collider->isFloor = true;
}

void Floor::update(float dt)
{
}

void Floor::set_position(vec3 pos)
{
    if (collider)
        collider->set_position(pos);
    drawable->set_position(pos);
}

void Floor::set_scale(float scale)
{
    drawable->set_scale(scale);
    if (collider)
    {
        collider->size.x = scale;
        collider->size.y = scale;
    }
}

void Floor::use_as_ceiling()
{
    //flip normal of collider
    //or just disable?
    if (collider)
    {
        collider->normal = {0, -1, 0};
    }
        
    //rotate mesh
    drawable->rotate({M_PI, 0, 0});
}

Drawable *Floor::get_drawable()
{
    return this->drawable;
}

void Floor::cleanup()
{
}

void Floor::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
        break;
    default:
        break;
    }
}

Collider *Floor::get_collider()
{
    return this->collider;
}