#include "damageimpactgenerator.h"

#include "worldobject/worldobject.h"

#include "utils/tostring.h"
#include "collision/voxelcollision.h"
#include "voxel/voxel.h"
#include "physics/physics.h"
#include "worldobject/worldobjectinfo.h"

void DamageImpactGenerator::parse(std::list<WorldObjectCollision>& worldObjectCollisions) {
    m_damageImpactAccumulator.clear();

    for(WorldObjectCollision& worldObjectCollision : worldObjectCollisions) {
        WorldObject* worldObjectA = worldObjectCollision.worldObjectA();
        WorldObject* worldObjectB = worldObjectCollision.worldObjectB();

        float massPerImpactA = worldObjectA->physics().mass() / worldObjectCollision.voxelCollisions().size();
        float massPerImpactB = worldObjectB->physics().mass() / worldObjectCollision.voxelCollisions().size();

        Transform targetTransformA = worldObjectA->physics().speed().moved(worldObjectA->transform(), 1.0f);
        Transform targetTransformB = worldObjectB->physics().speed().moved(worldObjectB->transform(), 1.0f);

        for(VoxelCollision& voxelCollision :  worldObjectCollision.voxelCollisions()) {
            glm::vec3 voxelSpeedA = targetTransformA.applyTo(glm::vec3(voxelCollision.a().voxel()->gridCell())) - worldObjectA->transform().applyTo(glm::vec3(voxelCollision.a().voxel()->gridCell()));
            glm::vec3 voxelSpeedB = targetTransformB.applyTo(glm::vec3(voxelCollision.b().voxel()->gridCell())) - worldObjectB->transform().applyTo(glm::vec3(voxelCollision.b().voxel()->gridCell()));


            std::cout << "Energy A = " << glm::length(voxelSpeedA*voxelSpeedA * massPerImpactA/2.0f)<< std::endl;
            std::cout << "Energy B = " << glm::length(voxelSpeedB*voxelSpeedB * massPerImpactB/2.0f)<< std::endl;
            std::cout << "Energy delta = " << (((massPerImpactA * massPerImpactB) / (2*(massPerImpactA + massPerImpactB))) * (glm::length((voxelSpeedA - voxelSpeedB)) * glm::length((voxelSpeedA - voxelSpeedB)))) << std::endl;

            if (voxelCollision.a().worldObject()->spawnState() != SpawnState::RemovalScheduled) {
                m_damageImpactAccumulator.parse(DamageImpact(voxelCollision.a().worldObject(),
                                                             voxelCollision.a().voxel(),
                                                             (voxelSpeedB - voxelSpeedA) * (massPerImpactB),
                                                             voxelCollision.b().worldObject()->collisionFieldOfDamage()));
            }
            if (voxelCollision.b().worldObject()->spawnState() != SpawnState::RemovalScheduled) {
                m_damageImpactAccumulator.parse(DamageImpact(voxelCollision.b().worldObject(),
                                                             voxelCollision.b().voxel(),
                                                             (voxelSpeedA - voxelSpeedB) * (massPerImpactA),
                                                             voxelCollision.a().worldObject()->collisionFieldOfDamage()));
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void DamageImpactGenerator::parse(std::list<DamageImpact>& damageImpacts) {
    for (DamageImpact& damageImpact : damageImpacts) {
          m_damageImpactAccumulator.parse(damageImpact);
    }
}

std::list<DamageImpact>& DamageImpactGenerator::damageImpacts() {
    return m_damageImpactAccumulator.accumulatables();
}

