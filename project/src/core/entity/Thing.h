#pragma once

/*
    Abstract class defining a "thing", defines pure virtual functions
*/
class Thing 
{
public:
    virtual ~Thing() = default;
    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void cleanup() = 0;
};