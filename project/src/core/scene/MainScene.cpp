#include "MainScene.h"
#include "SimpleGUI.h"
#include <stdlib.h>

void MainScene::init(Camera *camera)
{
    sgCreateStaticString(20, RES_Y - 20, "1-WASD, 2-WASDQE, 3-Third Person, 4-Topdown");
    sgSetBackgroundColor(0, 0, 0, 0);
    // A slider and a float display
    //  sgCreateStaticString(40, 80, "Slider and float display");
    //  sgCreateSlider(40, 100, 200, &testf, 5, 10);
    //  sgCreateDisplayFloat(40, 120, "Value: ", &testf);
    //  Input::set_lock_mouse(false);

    // Define directional light, it must come first!
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.1, 0.1, 0.03, 0.1);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    // Create player object
    player = new Player(&mailbox, camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);
    colliders.push_back(player->get_collider());
    lights.push_back(player->get_light());

    //Create goal object
    Goal* goal = new Goal(&mailbox);
    goal->init();
    goal->set_position(goalPos);
    drawables.push_back(goal->get_drawable());
    things.push_back(goal);
    lights.push_back(goal->get_light());

    // Initialise floor
    Floor *floor = new Floor(&mailbox);
    floor->init();
    floor->set_position({15, -0.6f, 15});
    floor->set_scale(50);
    drawables.push_back(floor->get_drawable());
    things.push_back(floor);
    colliders.push_back(floor->get_collider());

    // Initialise ceiling (scaled -y floor)
    Floor *ceiling = new Floor(&mailbox);
    ceiling->init();
    ceiling->use_as_ceiling();
    ceiling->set_position({15, 3, 15});
    ceiling->set_scale(50);
    drawables.push_back(ceiling->get_drawable());
    things.push_back(ceiling);
    colliders.push_back(ceiling->get_collider());

    // Create Maze
    create_maze();
    elapsed = 0;
}

void MainScene::update(float dt)
{
    Scene::update(dt);

    elapsed += dt;

    //check for goal
    vec3 tmp = player->get_position();
    if (!reachedGoal && NormSq(tmp - goalPos) < 10.f)
    {
        reachedGoal = true;
        debug_log("Goal reached\n");
        mailbox.notify(PLAYER_REACH_GOAL);
    }
}

void MainScene::on_notify(MailTopic topic, void *aux)
{
    // can recieve messages from things in the scene here
}

void MainScene::create_maze()
{
    int size = 20;
    float width = 2.f;
    float height = 3.f;
    for (int i = 0; i < size; ++i)
    {

        for (int j = 0; j < size; ++j)
        {
            vec3 pos = {(float)i * width, 0.0f, (float)j * width};
            int mazeVal = get_maze_value(i, j);
            if (mazeVal == 99) // player
            {
                player->set_position({pos.x, 0.5f ,pos.z});
                player->set_dir({1, 0, 0});
            }
            else if (mazeVal == 69) // goal
            {
                debug_log("Goal is at (%f, %f)\n", pos.x, pos.z);
            }
            else if (mazeVal == 0)
            {
                MazeWall *mazeWall = new MazeWall(&mailbox);
                mazeWall->init();
                mazeWall->set_height(height);
                mazeWall->set_width(width * 0.5f);
                mazeWall->set_position(pos);
                drawables.push_back(mazeWall->get_drawable());
                things.push_back(mazeWall);
                colliders.push_back(mazeWall->get_colliderLeft());
                colliders.push_back(mazeWall->get_colliderRight());
                colliders.push_back(mazeWall->get_colliderFront());
                colliders.push_back(mazeWall->get_colliderBack());

                bool left, right, front, back;

                left = get_maze_value(i, j - 1) || j - 1 < 0;
                right = get_maze_value(i, j + 1) || j + 1 >= size;
                front = get_maze_value(i + 1, j) || i + 1 >= size;
                back = get_maze_value(i - 1, j) || i - 1 < 0;
                mazeWall->set_active_colliders(left, right, front, back);
            }
            else if (mazeVal == 1 || mazeVal == 2)
            {
                /// is empty
                // if (j < 10) //print first 10 row path
                // {
                //     debug_log("%f %f %f\n", pos.x, pos.y, pos.z);
                // }
            }
            // Place random objects at random places
            else if (mazeVal == 3)
            {
                Obstacle *obs = new Obstacle(&mailbox);
                obs->init();
                obs->set_position({pos.x, pos.y - 0.7f, pos.z});
                drawables.push_back(obs->get_drawable());
                things.push_back(obs);
                colliders.push_back(obs->get_collider());
            }
            else if (mazeVal == 4)
            {
                TorchLight *torch = new TorchLight(&mailbox);
                torch->init();
                torch->set_position({pos.x, height - 0.5f, pos.z});
                auto dr = torch->get_drawables();
                drawables.push_back(dr[0]); // careful here, might want to append vector instead
                drawables.push_back(dr[1]); // but works fine for now
                things.push_back(torch);
                lights.push_back(torch->get_light());
            }
        }
    }
}

int MainScene::get_maze_value(int i, int j)
{
    // todo error check
    return maze_three[i][j];
}
