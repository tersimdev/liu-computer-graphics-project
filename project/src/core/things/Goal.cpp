#include "Goal.h"

void Goal::init()
{
    this->drawable = dh::create_from_model("spider2.obj", Transform(vec3(0), {0, M_PI * 0.5f, 0}));
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "spider2");
    drawable->setMaterial(m);
    drawable->set_scale(3);
    //default not drawn
    drawable->setDrawFlag(false);

    light = new Light;
    light->type = LightType::POINT;
    light->color = vec4(1, 0.2, 0.2, 0.3f);
    light->position = vec3(0); //set later

    mailbox->sub(PLAYER_REACH_GOAL, this);
}

void Goal::set_position(vec3 pos)
{
    drawable->set_position(pos + vec3(3, 0, 0));
    if (light)
        light->position = pos + vec3(0, 1, 0);
}

void Goal::update(float dt)
{
}

Drawable * Goal::get_drawable()
{
    return drawable;
}

Light *Goal::get_light()
{
    return light;
}

void Goal::cleanup()
{
}

void Goal::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
    case PLAYER_REACH_GOAL:
    {
        drawable->setDrawFlag(true);
        debug_log("spider says hello\n");
    }
    break;
    default:
        break;
    }
}