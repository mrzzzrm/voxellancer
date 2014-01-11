#include "inertiafollower.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "utils/tostring.h"


InertiaFollower::InertiaFollower(float directionalInertia, float angularInertia):
    m_directionalInertia(directionalInertia),
    m_angularInertia(angularInertia)
{

}

const glm::vec3& InertiaFollower::position() const {
    return m_position;
}

const glm::quat& InertiaFollower::orientation() const {
    return m_orientation;
}

void InertiaFollower::follow(const glm::vec3& targetPosition, const glm::quat& targetOrientation, float deltaSec) {
    m_position = glm::mix(m_position, targetPosition, m_directionalInertia * deltaSec);
    m_orientation = glm::slerp(m_orientation, targetOrientation, m_angularInertia * deltaSec);
}

