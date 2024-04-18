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
    MazeWall(ThingMailbox* mailbox, Camera* camera) : mailbox(mailbox), camera(camera) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    void translate(vec3 trl);
    void set_pos(vec3 pos);

    Drawable* get_drawable();
private:
    Drawable* drawable;
    ThingMailbox* mailbox;
    Camera* camera;



};