#include "TorchLight.h"

void TorchLight::init()
{
    this->drawable = dh::create_from_model("objCandle.obj", Transform({0, 0, 0}, {0, 0, 0}, {0.1, 0.1, 0.1}));
    Material *m = dh::create_material(ShaderProg::SIMPLE, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    //dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);
}

void TorchLight::set_position(vec3 pos)
{        
    drawable->set_position(pos);
}

void TorchLight::update(float dt)
{
}

Drawable *TorchLight::get_drawable()
{
    return this->drawable;
}

void TorchLight::cleanup()
{
}

void TorchLight::on_notify(MailTopic topic, void* aux)
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