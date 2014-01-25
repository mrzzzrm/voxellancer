#pragma once

#include <list>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "worldobject/worldobjecthandle.h"


class Ship;
class WorldObject;

class BoardComputer
{
public:
    BoardComputer(Ship& ship);
    
    void moveTo(const glm::vec3& position, bool deaccelerate = true);
    void rotateTo(const glm::vec3& position, const glm::vec3& up = glm::vec3(0, 0, 0));
    void shootBullet(const std::list<std::shared_ptr<WorldObjectHandle>>& targets);
    void shootRockets(std::shared_ptr<WorldObjectHandle> target);

protected:
    Ship& m_ship;

};

