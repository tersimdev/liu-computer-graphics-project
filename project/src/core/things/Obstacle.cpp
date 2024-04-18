#include "Obstacle.h"

void Obstacle::init()
{
    float scale = 0.3;
    vec3 pos = vec3(0);
    this->collider = new SphereCollider(pos, scale);
    
    this->drawable = dh::create_sphere(Transform(), 32, 32);
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);
    drawable->set_position(pos);
    drawable->set_scale(scale);
}

void Obstacle::update(float dt)
{
    drawable->set_position(collider->get_position());
}

void Obstacle::cleanup()
{
}

void Obstacle::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
    default:
        break;
    }
}

void Obstacle::set_position(vec3 pos)
{
    if (collider)
        collider->set_position(pos);
}

Drawable *Obstacle::get_drawable()
{
    return this->drawable;
}

Collider *Obstacle::get_collider()
{
    return this->collider;
}
