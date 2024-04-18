#include "MazeWall.h"

void MazeWall::init()
{
    this->drawable = dh::create_from_model("cube.obj", Transform({0, 0, -5}, {0, 0, 0}, {0.5, 0.5, 0.5}));
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);
}

void MazeWall::update(float dt)
{
//   drawable->rotate({0, 2 *dt, 0});
    if(camera)
    {
        camera->example_movement(dt);
    }
}

Drawable *MazeWall::get_drawable()
{
    return this->drawable;
}

void MazeWall::cleanup()
{
}

void MazeWall::on_notify(MailTopic topic, void* aux)
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