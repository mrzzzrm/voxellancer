#pragma once

#include "genericrocket.h"


/**
 *   A special Rocket that detonates before reaching the target and spawns normal Hornet Rockets
*/
class SplitRocket: public GenericRocket {
public:
    virtual void setTarget(WorldObject* targetObject) override;


protected:
    friend class SplitRocketTask;
    float detonationDistance();
    float detonationFieldOfAim();
    void detonate();
    void spawnChildren();
    virtual void spawnExplosion() override;

};

