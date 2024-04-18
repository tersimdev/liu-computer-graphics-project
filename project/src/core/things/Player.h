#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/SphereCollider.h"
#include "../entity/Rigidbody.h"
#include "../../util/Camera.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

/*
    Defines a player object. Handles logic like shooting input, movement, health
    Composes of entities like Drawables, colliders 
*/

class Player : public Thing
{
public:
    Player(ThingMailbox* mailbox, Camera* camera) : mailbox(mailbox), camera(camera) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    void set_position(vec3 pos);
    void set_cam_offset(vec3 camOffset);
    Drawable* get_drawable();
    Collider* get_collider();
private:
    Drawable* drawable;
    ThingMailbox* mailbox;
    Camera* camera;
    SphereCollider* collider;
    Rigidbody* rigidbody;

    vec3 position;
    vec3 camOffset; 

    void do_player_input(float dt);
};