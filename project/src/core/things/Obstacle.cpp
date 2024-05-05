#include "Obstacle.h"
#include <stdlib.h>

void Obstacle::init()
{
    Material *m;
    vec3 pos = {0, 2, 0};
    float scale = 0.8;
    autoInteract = false;
    // switch (3) // modulo num of types
    switch (rand() % 6) // modulo num of types
    {
    default:
    case 0:
        this->drawable = dh::create_sphere(Transform(), 32, 32);
        m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
        dh::attach_texture_to_material(m, 0, "birchwood");
        break;
    case 1:
        this->drawable = dh::create_from_model("monkey.obj", Transform());
        m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
        dh::attach_texture_to_material(m, 0, "ikea");
        break;

    case 2:
        this->drawable = dh::create_from_model("bunnyplus.obj", Transform());
        m = dh::create_material(ShaderProg::LIT, {0.8, 0.2, 0}, {0.8, 0.8, 0.8, 16});
        dh::attach_texture_to_material(m, 0, "conc");
        break;
    case 3:
    case 4:
    case 5:
        this->drawable = dh::create_from_model("spider2.obj", Transform(vec3(0), vec3(0, M_PI * 0.5f, 0)));
        m = dh::create_material(ShaderProg::LIT, {0.2, 0, 0}, {0.8, 0.8, 0.8, 256});
        scale = 0.4;
        autoInteract = true;
        break;
    }

    drawable->setMaterial(m);
    drawable->set_position(pos);
    drawable->set_scale(scale);
    this->collider = new SphereCollider(pos, scale * 0.5f);
    this->rigidbody = new Rigidbody(DYNAMIC, scale);
    this->collider->set_rigidbody(rigidbody);

    mailbox->sub(PLAYER_SAID_HELLO, this);
    mailbox->sub(PLAYER_LOCATION, this);
}

void Obstacle::update(float dt)
{
    vec3 pos = collider->get_position();
    // drawable->rotate({0, 2 * dt, 0});
    pos = rigidbody->update(dt, pos); // uncomment when obstacles are dynamic
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
    case PLAYER_LOCATION:
    {
        vec3 *pos = reinterpret_cast<vec3 *>(aux); // make sure this is not invalid!
        if (autoInteract)
            interact(*pos);
        break;
    }
    case PLAYER_SAID_HELLO:
    {
        vec3 *pos = reinterpret_cast<vec3 *>(aux); // make sure this is not invalid!
        interact(*pos);
        break;
    }
    default:
        break;
    }
}

void Obstacle::set_position(vec3 pos)
{
    if (collider)
        collider->set_position(pos);
    this->drawable->set_position(pos);
}

Drawable *Obstacle::get_drawable()
{
    return this->drawable;
}

Collider *Obstacle::get_collider()
{
    return this->collider;
}

void Obstacle::interact(vec3 pos)
{
    vec3 delta = (drawable->getTransform()->translation - pos);
    float dist = Norm(delta);
    if (dist <= interactRange)
    {
        vec3 v = delta / dist * 5;
        rigidbody->set_vel({v.x, 5, v.z});
        rigidbody->set_grav_scale(1);
    }
}
