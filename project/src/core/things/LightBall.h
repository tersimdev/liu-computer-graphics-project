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
    void on_notify(MailTopic topic) override;

    Drawable* get_drawable();
private:
    Drawable* drawable;
    Light* light;
};