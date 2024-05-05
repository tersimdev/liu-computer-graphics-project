#include "Player.h"

void Player::init()
{
    position = PLAYER_START_POS;
    float scale = 0.5f;

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

    camMode = WASD; // default
    // camMode = THIRDPER; // temp
    init_cam();

    // add a light to player
    light = new Light;
    light->type = LightType::POINT;
    light->color = vec4(0.9, 0.3, 0.1, 1.0f);
    light->position = position;
}

void Player::update(float dt)
{
    drawable->rotate({0, 2 * dt, 0});
    // get collided position
    position = collider->get_position();
    //restrict player height since terrain is flat
    if (camMode == WASD)
        position.y = playerHeight;
    // use cam to move player position
    do_player_input(dt);
    // update position of renderable and collider based on camera new pos
    drawable->set_position(position);
    collider->set_position(position);
    light->position = position + vec3(0, 1, 0);

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
    playerHeight = pos.y;
    drawable->set_position(pos);
    collider->set_position(pos);
}

Drawable *Player::get_drawable()
{
    return this->drawable;
}

void Player::set_dir(vec3 dir)
{
    if (camera)
        camera->set_dir(dir);
}

void Player::set_move_speed(float moveSpeed, float mouseSpeed)
{
    if (!camera)
        return;
    camera->set_move_speed(moveSpeed);
    camera->set_mouse_sens(mouseSpeed);
}

Collider *Player::get_collider()
{
    return this->collider;
}

vec3 Player::get_position()
{
    return position;
}

Light *Player::get_light()
{
    return this->light;
}

void Player::init_cam()
{
    camera->set_dir(DEFAULT_CAM_DIR);
    camera->set_up({0, 1, 0});
    camOffset = {0, 1, 0};
    set_move_speed(3, 0.2);
    Input::set_lock_mouse(true);
    drawable->setDrawFlag(false);
    switch (camMode)
    {
    default:
    case WASD:
        break;
    case WASDQE:        
        set_move_speed(10, 0.2); //move faster
        break;
    case THIRDPER:
        camOffset = {0, 0, 2};
        drawable->setDrawFlag(true);
        break;
    case TOPDOWN:
        camOffset = {0, 0, 10};
        camera->set_up({0, 0, -1});
        camera->set_dir({0, -1, 0});
        Input::set_lock_mouse(false);
        drawable->setDrawFlag(true);
        break;
    }
}

void Player::do_player_input(float dt)
{
    /*handle inputs below */

    // wasd qe movement
    vec3 dir = {0, 0, 0};
    vec2 deltaMouse = Input::get_mouse_delta();

    switch (camMode)
    {
    case WASD:
    case WASDQE:
    case THIRDPER:
        if (Input::get_action("fwd"))
            dir.z = -1;
        else if (Input::get_action("bck"))
            dir.z = 1;
        if (Input::get_action("right"))
            dir.x = 1;
        else if (Input::get_action("left"))
            dir.x = -1;
        if (camMode != WASD)
        {
            if (Input::get_action("up"))
                dir.y = 1;
            else if (Input::get_action("down"))
                dir.y = -1;
        }
        break;
    case TOPDOWN:
        if (Input::get_action("fwd"))
            dir.y = 1;
        else if (Input::get_action("bck"))
            dir.y = -1;
        if (Input::get_action("right"))
            dir.x = 1;
        else if (Input::get_action("left"))
            dir.x = -1;
        if (Input::get_action("up"))
            camOffset.z += dt * 10;
        else if (Input::get_action("down"))
            camOffset.z -= dt * 10;
        break;
    }

    position = camera->move_pos(position, dir, dt);

    vec3 offset = camOffset.y * camera->get_up() + camOffset.z * -camera->get_dir();
    camera->set_pos(position + offset);
    if (camMode != TOPDOWN)
        camera->rotate_cam(deltaMouse, dt);

    if (Input::get_action("shoot", false))
    {
        debug_log("pew pew pew\n");
        mailbox->notify(PLAYER_SAID_HELLO, &position);
    }

    if (Input::get_action("debug", false))
    {
        //print out current player position
        debug_log(">>>> PLAYER POS [%f, %f, %f]\n", position.x, position.y, position.z);
        //mailbox->notify(PLAYER_SAID_HELLO);
    }

    // switch camera modes
    if (Input::get_action("camMode1", false))
    {
        camMode = WASD;
        init_cam();
    }
    else if (Input::get_action("camMode2", false))
    {
        camMode = WASDQE;
        init_cam();
    }
    else if (Input::get_action("camMode3", false))
    {
        camMode = THIRDPER;
        init_cam();
    }
    else if (Input::get_action("camMode4", false))
    {
        camMode = TOPDOWN;
        init_cam();
    }
}
