#include "Player.h"

void Player::init()
{
    position = {0, 0, 0};
    camOffset = {0, 1, 2};
    float scale = 0.3;

    this->drawable = dh::create_sphere(Transform(), 32, 32);
    Material *m = dh::create_material(ShaderProg::LIT, {0.5, 0.5, 0.7}, {0.8, 0.8, 0.8, 256});
    dh::attach_texture_to_material(m, 0, "conc");
    m->tileFactor = 0.1f;
    drawable->setMaterial(m);
    drawable->set_position(position);
    drawable->set_scale(scale);

    this->collider = new SphereCollider(position, scale);
    this->rigidbody = new Rigidbody(KINEMATIC, scale);
    this->collider->set_rigidbody(rigidbody);
}

void Player::update(float dt)
{
    drawable->rotate({0, 2 * dt, 0});
    // get collided position
    position = collider->get_position();
    // use cam to move player position
    do_player_input(dt);
    // update position of renderable and collider based on camera new pos
    drawable->set_position(position);
    collider->set_position(position);
}

void Player::cleanup()
{
}

void Player::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
    default:
        break;
    }
}

void Player::set_position(vec3 pos)
{
    position = pos;
    drawable->set_position(position);
    collider->set_position(position);
}

Drawable *Player::get_drawable()
{
    return this->drawable;
}

Collider *Player::get_collider()
{
    return this->collider;
}

void Player::do_player_input(float dt)
{
    /*handle inputs below */

    // wasd qe movement
    vec3 dir = {0, 0, 0};
    vec2 deltaMouse = Input::get_mouse_delta();

    if (Input::get_action("fwd"))
        dir.z = -1;
    else if (Input::get_action("bck"))
        dir.z = 1;
    if (Input::get_action("right"))
        dir.x = 1;
    else if (Input::get_action("left"))
        dir.x = -1;
    if (Input::get_action("up"))
        dir.y = 1;
    else if (Input::get_action("down"))
        dir.y = -1;
    position = camera->move_pos(position, dir, dt);

    vec3 offset = camOffset.y * camera->get_up() + camOffset.z * -camera->get_dir();
    camera->set_pos(position + offset);
    camera->rotate_cam(deltaMouse, dt);

    if (Input::get_action("shoot", false))
    {
        // debug_log("pew pew pew\n");
        mailbox->notify(PLAYER_SAID_HELLO);
    }
}
