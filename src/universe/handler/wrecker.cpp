#include "wrecker.h"

#include "physics/physics.h"

#include "voxel/voxel.h"

#include "universe/helper/worldobjectmodification.h"

#include "worldobject/worldobject.h"
#include "worldobject/worldobjectinfo.h"


void Wrecker::detectWreckedObjects(std::list<WorldObjectModification>& worldObjectModifications) {
    m_wreckedObjects.clear();
    m_newWreckages.clear();

    for (WorldObjectModification& modification : worldObjectModifications) {
        WorldObject* object = modification.worldObject();

        if (object->isCrucialVoxelDestroyed()) {
            object->scheduleRemoval();
            createWreckFromObject(object);
        }
    }
}

void Wrecker::createWreckFromObject(WorldObject* object) {
    WorldObject* wreckage = new WorldObject(object->transform());

    wreckage->info().setName(object->info().name() + " - wreck");
    wreckage->physics().setSpeed(object->physics().speed());

    for (auto& pair : object->voxelMap()) {
        wreckage->addVoxel(new Voxel(*pair.second));
    }

    wreckage->spawn(object->sector());
}
