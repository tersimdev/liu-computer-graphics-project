#include "MainScene.h"
#include "SimpleGUI.h"
#include <stdlib.h>

void MainScene::init(Camera *camera)
{
	sgCreateStaticString(20, RES_Y - 20, "1-WASD, 2-WASDQE, 3-Third Person, 4-Topdown");
    sgSetBackgroundColor(0,0,0,0);
	//A slider and a float display
	// sgCreateStaticString(40, 80, "Slider and float display");
	// sgCreateSlider(40, 100, 200, &testf, 5, 10);
	// sgCreateDisplayFloat(40, 120, "Value: ", &testf);
	// Input::set_lock_mouse(false);


    // Create player object
    player = new Player(&mailbox, camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);
    colliders.push_back(player->get_collider());   


    // put more balls to test
    //this temp code should move into maze, 
    //ie for each maze cell chance to spawn an obstacle
    /*for (int i = 1; i <= 12; ++i)
    {
        Obstacle *temp = new Obstacle(&mailbox);
        temp->init();
        temp->set_position({(float)i, 0, -5});
        drawables.push_back(temp->get_drawable());
        things.push_back(temp);
        colliders.push_back(temp->get_collider());
    }*/

    //Initialise floor
    Floor *floor = new Floor(&mailbox);
    floor->init();
    drawables.push_back(floor->get_drawable());
    things.push_back(floor);
    colliders.push_back(floor->get_collider());

    
    //Create Maze
    for(int i=0; i<30; ++i)
    {
        
        for(int j=0; j<30; ++j)
        {
            MazeWall *temp = new MazeWall(&mailbox);
            vec3 pos = {(float)i, 0.0f, (float)j};
            if (temp->get_maze(i, j) == 1)
            {
                temp->init();
                temp->set_position(pos);
                drawables.push_back(temp->get_drawable());
                things.push_back(temp);
                colliders.push_back(temp->get_colliderLeft());
                colliders.push_back(temp->get_colliderRight());
                colliders.push_back(temp->get_colliderFront());
                colliders.push_back(temp->get_colliderBack());
            }
            
            // Place random objects at random places
            else if (temp->get_maze(i, j)==0 || temp->get_maze(i, j) == 2)
            {
                if(rand() % 1000 > 950)
                {
                    Obstacle *obs = new Obstacle(&mailbox);
                    obs->init();
                    obs->set_position(pos);
                    drawables.push_back(obs->get_drawable());
                    things.push_back(obs);
                    colliders.push_back(obs->get_collider());

                }
            }
        }

    }

    // todo Create enemy spawner

    // Define lighting
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.8, 0.8, 0.6, 0.2);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    // add point light to test
    Light *plt = new Light;
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
    things.push_back(lightBall2);
    elapsed = 0;
}

void MainScene::update(float dt)
{
    Scene::update(dt);

    if (Input::get_action("m"))
        lightBall->translate({dt*10, 0, 0});
    if (Input::get_action("n"))
        lightBall->translate({-dt*10, 0, 0});

    elapsed += dt;

    lightBall2->set_pos({sinf(elapsed) * 6 + 6,-0.5, -5});
}

void MainScene::on_notify(MailTopic topic, void *aux)
{
    //can recieve messages from things in the scene here
}
