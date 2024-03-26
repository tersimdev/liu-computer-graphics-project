#include "Scene.h"

void Scene::update(float dt)
{
    for (Thing *th : things)
    {
        th->update(dt);
    }
}

void Scene::cleanup()
{
    for (Light *l : lights)
    {
        delete l;
    }
    for (Drawable *d : drawables)
    {
        Material *m = d->getMaterial();
        if (m)
            delete m;
        delete d;
    }
    for (Thing *th : things)
    {
        delete th;
    }

    lights.clear();
    drawables.clear();
    things.clear();

    mailbox.cleanup();
}

std::vector<Light *>& Scene::get_lights()
{
    return lights;
}

std::vector<Drawable *> *Scene::get_drawables()
{
    return &drawables;
}

std::vector<Thing *> *Scene::get_things()
{
    return &things;
}
