#include "ThingMailbox.h"

#include <algorithm>

void ThingMailbox::notify(MailTopic topic, void* aux)
{
    if (subscribers.find(topic) == subscribers.end())
        return;
    std::vector<Thing*> list = subscribers[topic];
    for (Thing* o : list)
        o->on_notify(topic, aux);
}

void ThingMailbox::sub(MailTopic topic, Thing* thing)
{
    subscribers[topic].push_back(thing);
}

void ThingMailbox::unsub(MailTopic topic, Thing* thing)
{
    if (subscribers.find(topic) == subscribers.end())
        return;
    subscribers[topic].erase(
        std::remove(subscribers[topic].begin(), subscribers[topic].end(), thing), subscribers[topic].end());
}

void ThingMailbox::cleanup()
{
    for (auto [k,v] : subscribers)
    {
        v.clear();
    }
    subscribers.clear();
}
