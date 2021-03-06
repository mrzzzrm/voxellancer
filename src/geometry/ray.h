#pragma once

#include <glm/glm.hpp>

#include "abstractshape.h"


class Sphere;
class Transform;
template<typename T> class TAABB;

class Ray: public AbstractShape {
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    const glm::vec3& origin() const;
    void setOrigin(const glm::vec3& origin);

    const glm::vec3& direction() const;
    void setDirection(const glm::vec3& direction);

    virtual bool intersects(const Sphere& sphere) const override;
    virtual bool nearTo(const TAABB<int>& aabb) const override;
    virtual bool containedBy(const TAABB<int>& aabb) const override;

    static Ray fromTo(const glm::vec3& from, const glm::vec3& to);


protected:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};
