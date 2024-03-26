#pragma once

#include <vector>
#include "../entity/Thing.h"
/*
    Class to manage "things"
*/
class ThingManager
{
public:
    void init();
    void update(float dt);
    void cleanup(); 
    
    void add_obj(Thing* thing);
    void remove_obj(Thing* thing);
private:
    std::vector<Thing*> objArray;
};