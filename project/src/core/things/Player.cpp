#include "Player.h"

void Player::init()
{
    this->drawable = dh::create_from_model("bunnyplus.obj", Transform({0, 0, -5}, {0, -45, 0}, {0.5, 0.5, 0.5}));
    Material *m = dh::create_material("test", ShaderProg::LIT, {0.7, 0.2, 0.2}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);
}

void Player::update(float dt)
{
    drawable->rotate({0, 2 * dt, 0});
    if (camera)
    {
        camera->example_movement(dt);
    }
    if (Input::get_action("shoot"))
    {
        debug_log("pew pew pew\n");
    }
}

void Player::cleanup()
{
}

void Player::on_notify(MailTopic topic)
{
    // demo observer pattern
    switch (topic)
    {
    case PLAYER_SAID_HELLO:
    {
        debug_log("Player: Hello!\n");
    }
    break;
    default:
        break;
    }
}

Drawable *Player::get_drawable()
{
    return this->drawable;
}
