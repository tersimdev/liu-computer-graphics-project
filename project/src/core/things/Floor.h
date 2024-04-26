#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/PlaneCollider.h"
#include "../../util/Camera.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

class Floor : public Thing
{
public:
    Floor(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    void set_position(vec3 pos);
    Drawable* get_drawable();
    Collider* get_collider();
private:
    Drawable* drawable;
    PlaneCollider* collider;
    ThingMailbox* mailbox;

};