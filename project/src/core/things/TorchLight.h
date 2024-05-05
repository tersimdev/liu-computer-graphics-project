#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../../util/Camera.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"


#include "../entity/Light.h"
#include "LightBall.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

class TorchLight : public Thing
{
public:
    TorchLight(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;
    void set_position(vec3 pos);

    std::vector<Drawable *>get_drawables();
    Light* get_light();
private:
    Drawable* drawable;
    ThingMailbox* mailbox;

    LightBall* fireball;
    Light* fire;
};