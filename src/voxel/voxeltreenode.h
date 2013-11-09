#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <geometry/commontransform.h>
#include <geometry/sphere.h>

#include <voxel/voxel.h>
#include <voxel/voxelcluster.h>


class VoxeltreeNode
{
public:
    VoxeltreeNode(Voxelcluster &voxelcluster, const IAABB &gridAABB);
    virtual ~VoxeltreeNode();

    bool isAtomic() const;
    bool isLeaf() const;

    std::vector<VoxeltreeNode*> &subnodes();
    const std::vector<VoxeltreeNode*> &subnodes() const;

    Voxel *voxel();
    const Voxel *voxel() const;

    const IAABB &gridAABB() const;

    void insert(Voxel *voxel);
    void remove(const glm::ivec3 &cell);


protected:
    Voxelcluster &m_voxelcluster;
    Sphere m_absoluteBoundingSphere;
    IAABB m_gridAABB;

    CommonTransform m_transformCache;
    std::vector<VoxeltreeNode*> m_subnodes;
    Voxel *m_voxel;

    void split();
    void unsplit();
    void octuple();
};

