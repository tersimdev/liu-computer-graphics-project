#include "MazeWall.h"


void MazeWall::init()
{   
    vec3 pos = vec3(0.0f);
    this->drawable = dh::create_from_model("cube.obj", Transform(pos, {0, 0, 0}, {0.5, 0.5, 0.5}));
    Material *m = dh::create_material(ShaderProg::LIT, {1, 1, 1}, {0.8, 0.8, 0.8, 16});
    dh::attach_texture_to_material(m, 0, "conc");
    drawable->setMaterial(m);

}

int MazeWall::get_maze(int i, int j)
{ 
    int maze[20][20] = 
    {
        {1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,0,0,0,2,0,0,0,1,2,2,2,2,1,1,1},
        {1,1,1,1,1,1,1,1,2,1,1,1,1,2,1,1,2,1,1,1},
        {1,1,1,1,1,1,1,1,2,2,2,2,2,2,1,1,2,1,1,1},
        {1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,2,1,1,1},
        {1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,2,1,1,1},
        {1,1,1,0,1,1,2,2,2,2,2,2,2,2,2,2,2,0,0,1},
        {1,1,1,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,0,1},
        {1,1,0,1,1,1,2,1,1,2,2,2,2,2,2,1,1,1,0,1},
        {1,1,0,1,1,1,2,2,2,2,1,1,1,1,2,1,1,1,1,1},
        {1,1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1},
        {1,1,2,2,2,2,2,1,1,1,1,1,1,1,2,1,1,1,1,1},
        {1,1,2,1,1,1,2,2,2,2,2,2,2,2,2,0,0,0,1,1},
        {1,1,2,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
        {1,1,2,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1},
        {1,1,2,1,0,1,0,1,1,0,1,1,1,1,0,0,0,0,1,1},
        {1,1,2,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
        {1,1,2,2,2,2,2,2,2,2,0,0,0,0,1,1,1,1,1,1},
        {1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1}

    } ;

    return maze[i][j];

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

void MazeWall::set_position(vec3 pos)
{
    if (collider)
        collider->set_position(pos);
        this->colliderFront = new PlaneCollider({pos.x + 0.5f, pos.y, pos.z}, {1,0,0}, {0,1,0}, {0,0,1}, {0.5f,0.5f}); // Front Wall
        this->colliderRight = new PlaneCollider({pos.x, pos.y, pos.z+0.5f}, {0,0,1}, {1,0,0}, {0,1,0}, {0.5f,0.5f}); // Right Wall
        this->colliderBack = new PlaneCollider({pos.x - 0.5f, pos.y, pos.z}, {-1,0,0}, {0,1,0}, {0,0,1}, {0.5f,0.5f}); // Back Wall
        this->colliderLeft = new PlaneCollider({pos.x, pos.y, pos.z-0.5f}, {0,0,-1}, {1,0,0}, {0,1,0}, {0.5f,0.5f}); // Left Wall
        
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