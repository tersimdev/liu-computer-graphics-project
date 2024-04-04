#include "MainScene.h"


#include "../things/Player.h"
#include "../things/LightBall.h"
#include "SimpleGUI.h"

void MainScene::init(Camera *camera)
{
	sgCreateStaticString(20, RES_Y - 20, "WASDQE to move camera, NM to move lightball");
    sgSetBackgroundColor(0,0,0,0);
	//A slider and a float display
	// sgCreateStaticString(40, 80, "Slider and float display");
	// sgCreateSlider(40, 100, 200, &testf, 5, 10);
	// sgCreateDisplayFloat(40, 120, "Value: ", &testf);
	// Input::set_lock_mouse(false);


    // Create player object
    Player *player = new Player(&mailbox, camera);
    player->init();
    drawables.push_back(player->get_drawable());
    things.push_back(player);


    // put more players to test
    for (int i = 1; i <= 12; ++i)
    {
        Player *temp = new Player(&mailbox, nullptr);
        temp->init();
        temp->get_drawable()->translate({(float)i, 0, 0});
        drawables.push_back(temp->get_drawable());
        things.push_back(temp);
    }

    // todo Create maze

    // todo Create enemy spawner

    // Define lighting
    Light *dlt = new Light;
    dlt->type = LightType::DIR;
    dlt->color = vec4(0.8, 0.8, 0.6, 1);
    dlt->position = vec3(-1, -1, -0.5);
    lights.push_back(dlt);

    // add point light to test
    Light *plt = new Light;
    plt->type = LightType::POINT;
    plt->color = vec4(0, 0.8, 0.1, 1);
    plt->position = vec3(0, 0.5, -5);
    lights.push_back(plt);

    // add a drawable to point light to debug
    LightBall* lightBall = new LightBall(plt);
    lightBall->init();
    drawables.push_back(lightBall->get_drawable());
    things.push_back(lightBall);
    // example mailbox usage
    mailbox.sub(PLAYER_SAID_HELLO, lightBall); // lightBall listen to player
}

void MainScene::update(float dt)
{
    Scene::update(dt);
}

void MainScene::on_notify(MailTopic topic, void *aux)
{
    //can recieve messages from things in the scene here
}
