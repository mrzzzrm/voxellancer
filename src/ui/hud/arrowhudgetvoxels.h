#pragma once

#include <vector>

#include "voxel/voxelcluster.h"
#include "property/property.h"


class ObjectHudget;
class Ray;

class ArrowHudgetVoxels {
public:
    ArrowHudgetVoxels(ObjectHudget* hudget);
    ~ArrowHudgetVoxels();

    ObjectHudget* hudget();

    void draw();

    void setTargeted(bool targeted);

    void updateDirection(glm::vec3 direction);

    bool findPointOnEdge();

    virtual bool isAt(const Ray& ray) const;


protected:
    ObjectHudget* m_hudget;
    mutable VoxelCluster m_arrow;

    Property<float> m_arrowDistance;

    glm::vec3 m_targetPoint;


    bool findPoint();

    float vectorAngleToPlane(glm::vec3 vector, glm::vec3 planeNormal);
    float vectorAngleToVector(glm::vec3 vector, glm::vec3 vector2);
};

