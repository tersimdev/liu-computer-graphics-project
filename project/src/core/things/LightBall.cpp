#include "LightBall.h"

void LightBall::init()
{
    if (!light)
    {
        debug_error("No light source attached.\n");
        return;
    }
    this->drawable = dh::create_sphere(Transform(light->position, vec3(0), vec3(0.2)), 16, 16);
    this->drawable->setMaterial(dh::create_material(SIMPLE, light->color, {0, 0, 0, 0}));
}

void LightBall::update(float dt)
{
    //tmp controls
    // if (Input::get_action("m"))
    //     light->position.x += dt * 10;
    // if (Input::get_action("n"))
    //     light->position.x -= dt * 10;
    drawable->getTransform()->translation = light->position;
}

void LightBall::cleanup()
{
}

void LightBall::on_notify(MailTopic topic, void* aux)
{
    // demo observer pattern
    switch (topic)
    {
    case PLAYER_SAID_HELLO:
    {
        debug_log("LightBall: Player said Hello!\n");
    }
    break;
    default:
        break;
    }
}

void LightBall::translate(vec3 trl)
{
    light->position += trl;
}

void LightBall::set_pos(vec3 pos)
{
    light->position = pos;
}

Drawable *LightBall::get_drawable()
{
    return this->drawable;
}
