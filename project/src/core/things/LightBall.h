#pragma once

#include "../entity/Thing.h"
#include "../entity/Drawable.h"
#include "../entity/Light.h"
#include "../../util/Input.h"
#include "../../util/Debug.h"

#include "../system/DrawableHelper.h"

/*
    Defines a ball that is a point light
*/

class LightBall : public Thing
{
public:
    LightBall(Light* light) : light(light) {}

    void init() override;
    void update(float dt) override;
    void cleanup() override;
    void on_notify(MailTopic topic, void* aux) override;

    void translate(vec3 trl);
    void set_pos(vec3 pos);

    Drawable* get_drawable();
private:
    Drawable* drawable;
    Light* light;
};