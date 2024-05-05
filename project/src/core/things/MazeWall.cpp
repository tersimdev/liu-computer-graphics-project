#include "MazeWall.h"

void MazeWall::init()
{
    vec3 pos = vec3(0.0f);
    this->drawable = dh::create_from_model("cubeUV.obj", Transform(pos, {0, 0, 0}, {0.5f, 0.5f, 0.5f}));
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "wall");
    drawable->setMaterial(m);

    float colliderOffset = 0.5f; //based on size

    // give temp pos first, set inactive
    this->colliderLeft = new PlaneCollider(pos, {0, 0, -1}, {0, 1, 0}, {1, 0, 0}, {colliderOffset, colliderOffset}); // Left Wall
    this->colliderRight = new PlaneCollider(pos, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {colliderOffset, colliderOffset}); // Right Wall
    this->colliderFront = new PlaneCollider(pos, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {colliderOffset, colliderOffset}); // Front Wall
    this->colliderBack = new PlaneCollider(pos, {-1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {colliderOffset, colliderOffset}); // Back Wall

    set_active_colliders(false, false, false, false);
}

void MazeWall::update(float dt)
{
}

Drawable *MazeWall::get_drawable()
{
    return this->drawable;
}

void MazeWall::cleanup()
{
}

void MazeWall::on_notify(MailTopic topic, void *aux)
{
    // demo observer pattern
    switch (topic)
    {
    case PLAYER_BANG_WALL:
    {
        debug_log("MazeWall: Player bang Wall!\n");
    }
    break;
    default:
        break;
    }
}

void MazeWall::set_position(vec3 pos)
{
    float colliderOffset = drawable->getTransform()->scale.x; //based on size of cube
    this->colliderLeft->set_position({pos.x, pos.y, pos.z - colliderOffset});  // Left Wall
    this->colliderRight->set_position({pos.x, pos.y, pos.z + colliderOffset}); // Right Wall
    this->colliderFront->set_position({pos.x + colliderOffset, pos.y, pos.z}); // Front Wall
    this->colliderBack->set_position({pos.x - colliderOffset, pos.y, pos.z});  // Back Wall

    drawable->set_position(pos);
}

Collider *MazeWall::get_colliderLeft()
{
    return this->colliderLeft;
}

Collider *MazeWall::get_colliderRight()
{
    return this->colliderRight;
}

Collider *MazeWall::get_colliderFront()
{
    return this->colliderFront;
}

Collider *MazeWall::get_colliderBack()
{
    return this->colliderBack;
}

void MazeWall::set_height(float h)
{
    drawable->getTransform()->scale.y = h;
    this->colliderLeft->size.x = h;
    this->colliderRight->size.x = h;
    this->colliderFront->size.x = h;
    this->colliderBack->size.x = h;
    // scale works for now, but should offset too
}

void MazeWall::set_width(float w)
{
    //preserve the y
    drawable->getTransform()->scale.x = w;
    drawable->getTransform()->scale.z = w;

    // update collider scale as well
    this->colliderLeft->size.y = w;
    this->colliderRight->size.y = w;
    this->colliderFront->size.y = w;
    this->colliderBack->size.y = w;

    //update collider offset by reset pos
    set_position(drawable->getTransform()->translation);
}

void MazeWall::set_active_colliders(bool left, bool right, bool front, bool back)
{
    this->colliderLeft->set_active(left);
    this->colliderRight->set_active(right);
    this->colliderFront->set_active(front);
    this->colliderBack->set_active(back);
}