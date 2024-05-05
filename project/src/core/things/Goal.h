#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../../util/Debug.h"
#include "../entity/Light.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

class Goal : public Thing
{
public:
    Goal(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;
    void set_position(vec3 pos);

    Drawable * get_drawable();
    Light* get_light();
private:
    Drawable* drawable;
    ThingMailbox* mailbox;
    Light* light;
};