#include "voxeltree.h"

#include "utils/tostring.h"

#include "worldobject/worldobject.h"

#include "voxel.h"
#include "voxeltreenode.h"


VoxelTree::VoxelTree(WorldObject* worldObject):
    m_shadowRoot(new VoxelTreeNode(0, this, nullptr, GridAABB(glm::ivec3(0, 0, 0), glm::ivec3(0, 0, 0)))),
    m_currentRoot(m_shadowRoot),
    m_worldObject(worldObject)
{
}

VoxelTree::~VoxelTree() {
    delete m_shadowRoot;
}

VoxelTreeNode* VoxelTree::root() {
    return m_currentRoot;
}

void VoxelTree::insert(Voxel* voxel) {
    while(!m_currentRoot->gridAABB().contains(voxel->gridCell())) {
        if(m_currentRoot->parent() != nullptr) {
            m_currentRoot = m_currentRoot->parent();
        } else {
            assert(m_currentRoot == m_shadowRoot);

            m_shadowRoot = new VoxelTreeNode(this, GridAABB(glm::ivec3(0, 0, 0), m_shadowRoot->gridAABB().urb()*2 + glm::ivec3(1, 1, 1)), m_shadowRoot);
            m_currentRoot = m_shadowRoot;
        }
    }

    m_currentRoot->insert(voxel);
}

void VoxelTree::remove(Voxel* voxel) {
    assert(voxel->voxelTreeNode());

    voxel->voxelTreeNode()->remove(voxel);

    if (m_currentRoot->subnodes().size() == 1) {
        m_currentRoot = m_currentRoot->subnodes().front();
    }
}

WorldObject* VoxelTree::worldObject() {
    return m_worldObject;
}

bool VoxelTree::isInstanced() {
    return false;
}


InstancedVoxelTree::InstancedVoxelTree(IVoxelTree& prototype):
    m_prototype(prototype)
{

}

void InstancedVoxelTree::insert(Voxel* voxel) {
    assert(0);
}

void InstancedVoxelTree::remove(Voxel* voxel) {
    assert(0);
}

bool InstancedVoxelTree::isInstanced() {
    return true;
}

VoxelTreeNode* InstancedVoxelTree::root() {
    return m_prototype.root();
}
