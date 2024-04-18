#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/Material.h"
#include "../entity/Light.h"
#include "../entity/Collider.h"
#include "../../util/Camera.h"
#include <vector>

#include "../system/DrawableHelper.h"
#include "../system/ThingMailbox.h"

/*
    Base class defining a scene construct. Inherit to create new scenes.
*/
class Scene : public Thing
{
public:
    virtual ~Scene() = default;
    void init() override; //override Thing
    virtual void init(Camera* camera) = 0; //inherit from thing but keep it abstract
    virtual void update(float dt);
    virtual void cleanup();
    virtual void on_notify(MailTopic topic, void* aux) = 0;


    std::vector<Light*>& get_lights();
    std::vector<Drawable*>* get_drawables();
    std::vector<Thing*>* get_things();
    std::vector<Collider*>* get_colliders();
protected:
    std::vector<Light*> lights; //0th is dirLight
    std::vector<Drawable*> drawables;
    std::vector<Thing*> things;
    std::vector<Collider*> colliders;

    ThingMailbox mailbox;
};