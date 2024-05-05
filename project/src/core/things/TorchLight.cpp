#include "TorchLight.h"

void TorchLight::init()
{
    this->drawable = dh::create_from_model("objCandle.obj", Transform({0, 0, 0}, {0, 0, 0}, {0.1, 0.1, 0.1}));
    Material *m = dh::create_material(ShaderProg::SIMPLE, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    drawable->setMaterial(m);

    fire = new Light;
    fire->type = LightType::POINT;
    fire->color = vec4(0.9, 0.2, 0.1, 0.3);
    fire->position = vec3(0); //set later

    fireball = new LightBall(fire);
    fireball->init();
}

void TorchLight::set_position(vec3 pos)
{
    drawable->set_position(pos);
    if (fire)
        fire->position = pos;
}

void TorchLight::update(float dt)
{
    //shld not be necesasry but
    fireball->update(dt);
}

std::vector<Drawable *> TorchLight::get_drawables()
{
    return {this->drawable, fireball->get_drawable()};
}

Light *TorchLight::get_light()
{
    return fire;
}

void TorchLight::cleanup()
{
}

void TorchLight::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
    case DA_WAE:
    {
        debug_log("Torch Light: LET ME SHOW YOU DA WAE!\n");
    }
    break;
    default:
        break;
    }
}