#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/SphereCollider.h"
#include "../entity/Rigidbody.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

/*
    Defines a Obstacle object. It is an object that blocks players path and has a collider.
    Also intended to have rigidbody physics.

    TODO: add random drawwable instead of always sphere
*/

class Obstacle : public Thing
{
public:
    Obstacle(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    void set_position(vec3 pos);
    Drawable* get_drawable();
    Collider* get_collider();
private:
    ThingMailbox* mailbox;
    Drawable* drawable;
    SphereCollider* collider;
    Rigidbody* rigidbody;
};