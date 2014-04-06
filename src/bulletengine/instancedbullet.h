#pragma once

#include <string>

#include "equipment/weapons/bullet.h"


class InstancedBullet : public Bullet {
public:
    struct Data {
        glm::vec3 v_creationPosition;
        glm::vec3 v_creationEulers;
        glm::vec3 v_directionalSpeed;
        glm::vec3 v_angularSpeed;
        float v_creationTime;
        float v_deathTime;
        glm::vec4 v_color;
        float v_emissiveness;
    };

public:
    InstancedBullet(const std::string& name);

    const std::string& name() const;

    int bufferSlot() const;
    void setBufferSlot(int bufferSlot);

    Data* data();

    virtual void update(float deltaSec) override;

    virtual Transform& transform() override;
    virtual void setTransform(const Transform& transform) override;

    virtual void setSpeed(const Speed& speed) override;

    virtual void spawn() override;
    virtual void remove() override;

    virtual float length() override;


protected:
    std::string m_name;
    int m_bufferSlot;
    Data m_data;
};
