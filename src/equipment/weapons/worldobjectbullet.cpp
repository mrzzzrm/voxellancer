#include "worldobjectbullet.h"

#include "collision/collisionfilterignoringcreator.h"

#include "physics/physics.h"

#include "voxel/voxelclusterbounds.h"

#include "world/god.h"
#include "world/world.h"

#include "worldobject/worldobjectinfo.h"


WorldObjectBullet::WorldObjectBullet() {
    collisionFilter().setCollideableWith(WorldObjectType::Bullet, false);

    m_info->setShowOnHud(false);
    m_info->setCanLockOn(false);

    m_physics->setDirectionalDampening(unnamedProperty(0.0f));
    m_physics->setAngularDampening(unnamedProperty(0.0f));
}

Transform& WorldObjectBullet::transform() {
    return WorldObject::transform();
}

void WorldObjectBullet::setTransform(const Transform& transform) {
    WorldObject::setTransform(transform);
}

void WorldObjectBullet::setSpeed(const Speed& speed) {
    physics().setSpeed(speed);
}

void WorldObjectBullet::setCreator(WorldObject* creator) {
    Bullet::setCreator(creator);

    CollisionFilterIgnoringCreator* newCollisionFilter = new CollisionFilterIgnoringCreator(
        this,
        m_creator,
        collisionFilter().collisionMask()
    );

    setCollisionFilter(newCollisionFilter);
}

WorldObjectType WorldObjectBullet::objectType() const {
    return WorldObjectType::Bullet;
}

void WorldObjectBullet::update(float deltaSec) {
    Bullet::update(deltaSec);
    WorldObject::update(deltaSec);
}

void WorldObjectBullet::remove() {
    World::instance()->god().scheduleRemoval(this);
}

void WorldObjectBullet::spawn() {
    World::instance()->god().scheduleSpawn(this);
}

float WorldObjectBullet::length() {
    return bounds().minimalGridAABB().extent(ZAxis) * transform().scale();
}

bool WorldObjectBullet::passiveForCollisionDetection() {
    return true;
}

