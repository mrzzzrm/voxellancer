#include "engine.h"


Engine::Engine(const std::string& key):
    WorldObjectEquipment(key),
    m_trailGenerator(this)
{
}

EngineSlot* Engine::engineSlot() {
    return m_engineSlot;
}

void Engine::setEngineSlot(EngineSlot* engineSlot) {
    m_engineSlot = engineSlot;
}

void Engine::update(float deltaSec) {
    m_trailGenerator.update(deltaSec);
}

const EngineState& Engine::state() const {
    return m_state;
}

void Engine::setState(const EngineState& state) {
    m_state = state;
}

Acceleration Engine::currentAcceleration() const {
    return power().accelerationAt(m_state);
}

