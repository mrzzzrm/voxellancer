#pragma once

#include <functional>

#include <glm/glm.hpp>
#include "geometry/sphere.h"
#include "property/property.h"


class VoxelCluster;
class VoxelTreeNode;
class WorldObject;

namespace std {
    template<> struct hash<glm::ivec3>
    {
        std::size_t operator()(const glm::ivec3 &v) const {
            return (v.x ^ (v.y << 8) ^ (v.z << 16))*31;
        }
    };
}


class Voxel
{
public:
    Voxel(const glm::ivec3& gridCell, const int color = 0xFFFFFF, float mass = defaultMass(), float hp = defaultHp());
    Voxel(const Voxel& other);
    virtual ~Voxel();

    const glm::ivec3 &gridCell() const;

    VoxelTreeNode *voxelTreeNode();
    void setVoxelTreeNode(VoxelTreeNode* voxelTreeNode);

    virtual void addToCluster(VoxelCluster *cluster);
    virtual void addToObject(WorldObject *object);

    int color() const;

    float hp() const;
    void applyDamage(float deltaHp);

    float normalizedMass() const;

    // These hooks apply only for WorldObjects and do not need to be called by pqure VoxelClusters
    virtual void onRemoval();
    virtual void onDestruction();



protected:
    glm::ivec3 m_gridCell;
    VoxelTreeNode *m_voxelTreeNode;
    int m_color;
    float m_hp;
    float m_normalizedMass;

    static Property<float>* s_defaultMass;
    static Property<float>* s_defaultHp;

    static float defaultMass();
    static float defaultHp();
};

