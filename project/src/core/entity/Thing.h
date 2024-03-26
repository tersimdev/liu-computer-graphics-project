#pragma once

/*
    Abstract class defining a "thing", defines pure virtual functions
*/
class Thing 
{
protected:
    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void cleanup() = 0;
};