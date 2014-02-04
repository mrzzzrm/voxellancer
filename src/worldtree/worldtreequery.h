#pragma once

#include <set>
#include <functional>

#include "geometry/abstractshape.h"


class Voxel;
class WorldTree;
class WorldTreeNode;
class WorldTreeGeode;
class WorldObject;
class CollisionFilter;

class WorldTreeQuery {
public:
    WorldTreeQuery(WorldTree* worldTree, const AbstractShape* shape, WorldTreeNode* nodeHint = nullptr, CollisionFilter* collisionFilter = nullptr);

    bool areGeodesNear();
    std::set<WorldTreeGeode*> nearGeodes();

    bool areVoxelsIntersecting();
    std::set<Voxel*> intersectingVoxels();

    std::set<WorldObject*> intersectingWorldObjects();


protected:
    WorldTree* m_worldTree;
    WorldTreeNode* m_nodeHint;
    WorldTreeNode* m_startNode;
    CollisionFilter* m_collisionFilter;
    const AbstractShape* m_shape;
    bool m_queryInterrupted;

    WorldTreeNode* getQueryRoot(WorldTreeNode* node = nullptr) const;
    void query(WorldTreeNode* node, std::function<void(WorldTreeGeode*)> onGeodeInteraction);
};

