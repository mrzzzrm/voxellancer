#include "instancedbullet.h"

#include "bulletengine/bulletengine.h"

#include "world/world.h"


InstancedBullet::InstancedBullet(const std::string& name):
    m_name(name),
    m_bufferSlot(-1)
{
}

const std::string& InstancedBullet::name() const {
    return m_name;
}

int InstancedBullet::bufferSlot() const {
    return m_bufferSlot;
}

void InstancedBullet::setBufferSlot(int bufferSlot) {
    m_bufferSlot = bufferSlot;
}

InstancedBullet::Data* InstancedBullet::data() {
    return &m_data;
}

Transform& InstancedBullet::transform() {

}

void InstancedBullet::setTransform(const Transform& transform) {

}

void InstancedBullet::setSpeed(const Speed& speed) {

}

void InstancedBullet::update(float deltaSec) {

}

void InstancedBullet::spawn() {
    World::instance()->bulletEngine().add(this);
}

void InstancedBullet::remove() {
    World::instance()->bulletEngine().remove(this);
}

float InstancedBullet::length() {

}
