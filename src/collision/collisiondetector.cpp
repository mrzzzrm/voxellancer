#include "collisiondetector.h"

#include <set>
#include <list>
#include <iostream>

#include "utils/tostring.h"
#include "voxel/voxelcluster.h"


CollisionDetector::CollisionDetector(Worldtree &worldtree, VoxelCluster &voxelcluster):
    m_worldtree(worldtree),
    m_voxelcluster(voxelcluster)
{

}

CollisionDetector::~CollisionDetector() {

}

const std::list<Collision> &CollisionDetector::checkCollisions() {
    assert(m_voxelcluster.geode() != nullptr);

    m_collisions.clear();

    std::set<WorldtreeGeode*> possibleColliders = m_worldtree.geodesInAABB(m_voxelcluster.geode()->aabb(), m_voxelcluster.geode()->containingNode());
    possibleColliders.erase(m_voxelcluster.geode());

    std::cout << "Possible colliders: " << possibleColliders.size() << std::endl;

    for(WorldtreeGeode *possibleCollider : possibleColliders) {
        assert(possibleCollider->voxelcluster());
        checkCollisions(&m_voxelcluster.voxeltree(), &possibleCollider->voxelcluster()->voxeltree());
    }

    return m_collisions;
}

void CollisionDetector::checkCollisions(VoxeltreeNode* nodeA, VoxeltreeNode* nodeB) {
    if(nodeA->isLeaf() && nodeA->voxel() == nullptr) {
        return;
    }
    if(nodeB->isLeaf() && nodeB->voxel() == nullptr) {
        return;
    }

    if(nodeA->boundingSphere().intersects(nodeB->boundingSphere())) {
        std::cout << "  Intersecting nodes" << std::endl;
        if(nodeA->isLeaf() && nodeB->isLeaf()) {
            if(nodeA->voxel() != nullptr && nodeB->voxel() != nullptr) {
                m_collisions.push_back(Collision());
                std::cout << "Collision detected between voxel at " << toString(nodeA->gridAABB()) << " and " << toString(nodeB->gridAABB()) << std::endl;
            }
        }
        else {
            std::vector<VoxeltreeNode*> nodesA, nodesB;

            if(nodeA->isLeaf()) {
                nodesA.push_back(nodeA);
            }
            else {
                nodesA = nodeA->subnodes();
            }

            if(nodeB->isLeaf()) {
                nodesB.push_back(nodeB);
            }
            else {
                nodesB = nodeB->subnodes();
            }

            for(VoxeltreeNode *nodeA : nodesA) {
                for(VoxeltreeNode *nodeB : nodesB) {
                    checkCollisions(nodeA, nodeB);
                }
            }
        }
    }
}


