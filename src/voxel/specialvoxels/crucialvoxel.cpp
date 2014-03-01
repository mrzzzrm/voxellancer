#include "crucialvoxel.h"

#include "property/property.h"

#include "voxel/voxelcluster.h"

#include "worldobject/worldobject.h"


CrucialVoxel::CrucialVoxel(const glm::ivec3& gridCell, int index):
    SpecialVoxel(gridCell, Property<uint32_t>::get("voxels.crucial.color"), index, Property<float>::get("voxels.crucial.mass"), Property<float>::get("voxels.crucial.hp"))
{
}

void CrucialVoxel::addToObject(WorldObject* worldObject){
    assert(worldObject->crucialVoxel() == nullptr);

    SpecialVoxel::addToObject(worldObject);
    worldObject->setCrucialVoxel(m_gridCell);
}

void CrucialVoxel::onRemoval(){
    //TODO: Destroy ship / make wreckage?
}

void CrucialVoxel::onDestruction(const WorldObject* owner) {
    SpecialVoxel::onDestruction(owner);
}

