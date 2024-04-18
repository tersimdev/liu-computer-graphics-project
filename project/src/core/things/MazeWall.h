#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../../util/Camera.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

class MazeWall : public Thing
{
public:
    MazeWall(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    Drawable* get_drawable();
private:
    Drawable* drawable;
    ThingMailbox* mailbox;



};