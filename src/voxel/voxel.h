#pragma once

#include <glm/glm.hpp>
#include "geometry/sphere.h"


typedef glm::detail::tvec3<unsigned char> cvec3;
 
struct VoxelHash
{
    std::size_t operator()(const cvec3& v) const {
        return v.x + (v.y << 8) + (v.z << 16);
    }
};


class VoxelCluster;

class Voxel
{
public:
    Voxel();
    Voxel(cvec3 gridCell, cvec3 color);
    virtual ~Voxel();

    // TODO: extract to PhysicalVoxel
    VoxelCluster *voxelCluster();
    void setVoxelCluster(VoxelCluster *cluster);

    const cvec3 &gridCell() const;
    //void setGridCell(const cvec3 &cell);

    const cvec3 &color() const;
    //void setColor(const cvec3 &color);


protected:
    VoxelCluster *m_voxelCluster;
    cvec3 m_gridCell;
    cvec3 m_color;
};

