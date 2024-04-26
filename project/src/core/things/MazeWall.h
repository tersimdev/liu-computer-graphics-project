#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/PlaneCollider.h"
#include "../../util/Camera.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"
#include <vector>

class MazeWall : public Thing
{
public:
    MazeWall(ThingMailbox* mailbox) : mailbox(mailbox) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;
    void set_position(vec3 pos);
    int get_maze(int i, int j);

    Collider* get_colliderLeft();
    Collider* get_colliderRight();
    Collider* get_colliderFront();
    Collider* get_colliderBack();
    Drawable* get_drawable();
    
    
private:
    Drawable* drawable;
    PlaneCollider* collider;
    PlaneCollider* colliderLeft;
    PlaneCollider* colliderRight;
    PlaneCollider* colliderFront;
    PlaneCollider* colliderBack;
    ThingMailbox* mailbox;
    

};