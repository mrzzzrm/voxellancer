#pragma once

#include "utils/aabbrenderer.h"



class WorldTreeRenderer {
public:
    WorldTreeRenderer();
    ~WorldTreeRenderer();

    void draw(const Camera& camera);


protected:
    std::unique_ptr<AABBRenderer> m_renderer;

    void poll(WorldTreeNode* node);
};

