#include "hardpointvoxel.h"

#include "property/property.h"

#include "voxel/voxelcluster.h"

#include "equipment/hardpoint.h"
#include "equipment/weapon.h"
#include "worldobject/worldobject.h"
#include "worldobject/worldobjectcomponents.h"


HardpointVoxel::HardpointVoxel(const glm::ivec3& gridCell, int index):
    SpecialVoxel(gridCell, index, Property<uint32_t>::get("voxels.hardpoint.color"), Property<float>::get("voxels.hardpoint.mass"), Property<float>::get("voxels.hardpoint.hp")),
    m_hardpoint(nullptr)
{

}

Visuals HardpointVoxel::visuals() const {
    return Visuals(
        m_hardpoint->weapon() ? m_hardpoint->weapon()->visuals() : SpecialVoxel::visuals()
    );
}

void HardpointVoxel::addToObject(WorldObject* worldObject) {
    SpecialVoxel::addToObject(worldObject);
    assert(m_hardpoint == nullptr);
    m_hardpoint = std::make_shared<Hardpoint>(&worldObject->components(), this);
    worldObject->components().addHardpoint(m_hardpoint);
}

void HardpointVoxel::onRemoval() {
    if (m_hardpoint) {
        m_hardpoint->onVoxelRemoval();
        m_hardpoint = nullptr;
    }
    SpecialVoxel::onRemoval();
}

void HardpointVoxel::onDestruction(const WorldObject* owner) {
    //Drop Ammo?
    SpecialVoxel::onDestruction(owner);
}

