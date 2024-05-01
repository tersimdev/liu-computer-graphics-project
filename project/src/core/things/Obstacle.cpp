#include "Obstacle.h"
#include <stdlib.h>

void Obstacle::init()
{
    float scale = 0.3;
    vec3 pos = vec3(0);
    this->collider = new SphereCollider(pos, scale);
    this->rigidbody = new Rigidbody(DYNAMIC, scale);
    this->collider->set_rigidbody(rigidbody);
    
    switch(rand() % 3)
    {
        case 0:
            this->drawable = dh::create_sphere(Transform(), 32, 32);
        break;

        case 1:
            this->drawable = dh::create_from_model("monkey.obj", Transform({0, -2, 0}, {0, 0, 0}, {0.1, 0.1, 0.1}));
        break;

        case 2:
            this->drawable = dh::create_from_model("bunnyplus.obj", Transform({0, -2, 0}, {0, 0, 0}, {0.1, 0.1, 0.1}));
        break;
    }
    
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "banana");
    drawable->setMaterial(m);
    drawable->set_position(pos);
    drawable->set_scale(scale);
}

void Obstacle::update(float dt)
{
    vec3 pos = collider->get_position();
    drawable->rotate({0, 2 * dt, 0});
    pos = rigidbody->update(dt, pos); //uncomment when obstacles are dynamic
    collider->set_position(pos);
    drawable->set_position(pos);
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
