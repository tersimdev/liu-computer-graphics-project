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
    dlt->color = vec4(0.3, 0.3, 0.233, 0.15);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    // Create player object
    player = new Player(&mailbox, camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);
    colliders.push_back(player->get_collider());

    // Initialise floor
    Floor *floor = new Floor(&mailbox);
    floor->init();
    floor->set_position({15, -0.6f, 15});
    floor->set_scale(18);
    drawables.push_back(floor->get_drawable());
    things.push_back(floor);
    colliders.push_back(floor->get_collider());

    // Initialise ceiling (scaled -y floor)
    Floor *ceiling = new Floor(&mailbox);
    ceiling->init();
    ceiling->use_as_ceiling();
    ceiling->set_position({15, 3, 15});
    ceiling->set_scale(18);
    drawables.push_back(ceiling->get_drawable());
    things.push_back(ceiling);
    colliders.push_back(ceiling->get_collider());

    // Create Maze
    int size = 20;
    for (int i = 0; i < size; ++i)
    {

        for (int j = 0; j < size; ++j)
        {
            MazeWall *temp = new MazeWall(&mailbox);
            vec3 pos = {(float)i, 0.0f, (float)j};
            int mazeVal = temp->get_maze(i, j);
            if (mazeVal == 0)
            {
                temp->init();
                temp->set_position(pos);
                drawables.push_back(temp->get_drawable());
                things.push_back(temp);
                colliders.push_back(temp->get_colliderLeft());
                colliders.push_back(temp->get_colliderRight());
                colliders.push_back(temp->get_colliderFront());
                colliders.push_back(temp->get_colliderBack());

                bool left, right, front, back;

                left = temp->get_maze(i, j - 1) || j - 1 < 0;
                right = temp->get_maze(i, j + 1) || j + 1 >= size;
                front = temp->get_maze(i + 1, j) || i + 1 >= size;
                back = temp->get_maze(i - 1, j) || i - 1 < 0;
                temp->set_active_colliders(left, right, front, back);
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
                torch->set_position({(float)i, 0.5f, (float)j});
                auto dr = torch->get_drawables();
                drawables.push_back(dr[0]); // careful here, might want to append vector instead
                drawables.push_back(dr[1]); // but works fine for now
                things.push_back(torch);
                lights.push_back(torch->get_light());
            }
        }
    }

    // todo Create enemy spawner

    // add point light to test
    /*Light *plt = new Light;
    plt->type = LightType::POINT;
    plt->color = vec4(0, 0.8, 0.1, 1);
    plt->position = vec3(0, 0.5, -5);
    lights.push_back(plt);

    // add a drawable to point light to debug
    lightBall = new LightBall(plt);
    lightBall->init();
    drawables.push_back(lightBall->get_drawable());
    things.push_back(lightBall);
    // example mailbox usage
    mailbox.sub(PLAYER_SAID_HELLO, lightBall); // lightBall listen to player

    //second light ball to animate
    plt = new Light;
    plt->type = LightType::POINT;
    plt->color = vec4(0.8, 0.2, 0.8, 1);
    plt->position = vec3(0, -0.5, -5);
    lights.push_back(plt);
    lightBall2 = new LightBall(plt);
    lightBall2->init();
    drawables.push_back(lightBall2->get_drawable());
    things.push_back(lightBall2);*/
    elapsed = 0;
}

void MainScene::update(float dt)
{
    Scene::update(dt);

    /*    if (Input::get_action("m"))
            lightBall->translate({dt*10, 0, 0});
        if (Input::get_action("n"))
            lightBall->translate({-dt*10, 0, 0});

        elapsed += dt;

        lightBall2->set_pos({sinf(elapsed) * 6 + 6,-0.5, -5});*/
}

void MainScene::on_notify(MailTopic topic, void *aux)
{
    // can recieve messages from things in the scene here
}
