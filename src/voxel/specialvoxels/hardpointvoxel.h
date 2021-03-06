#pragma once

#include "display/rendering/visuals.h"

#include "voxel/specialvoxel.h"


class Hardpoint;

class HardpointVoxel: public SpecialVoxel {
public:
    HardpointVoxel(const glm::ivec3& cell, int index);

    virtual Visuals visuals() const override;

    virtual void addToObject(WorldObject* object) override;

    virtual void onRemoval() override;
    virtual void onDestruction() override;


protected:
    std::shared_ptr<Hardpoint> m_hardpoint;
};

