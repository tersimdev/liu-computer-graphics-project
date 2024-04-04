#pragma once

#include <vector>
#include <unordered_map>
#include "../entity/Thing.h"
#include "../entity/MailTopic.h"
/*
    Class to manage message passing between "things"
    Things can subscribe to a certain topic, and recieve alerts for it
    Things can also post alerts
*/

class ThingMailbox
{
public:
    void notify(MailTopic topic, void* aux = nullptr);
    void sub(MailTopic topic, Thing* thing);
    void unsub(MailTopic topic, Thing* thing);

    void cleanup(); //unsub all
private:
    std::unordered_map<MailTopic, std::vector<Thing*>> subscribers;
};