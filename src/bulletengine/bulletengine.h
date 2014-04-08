#pragma once

#include <memory>
#include <string>
#include <unordered_map>


class BulletEngineRenderer;
class Camera;
class InstancedBullet;
class InstancedBulletPrototype;

class BulletEngine {
public:
    BulletEngine();
    ~BulletEngine();

    void add(InstancedBullet* bullet);
    void remove(InstancedBullet* bullet);

    InstancedBullet* createBullet(const std::string& name);

    void update(float deltaSec);
    void draw(const Camera& camera);


protected:
    std::unique_ptr<BulletEngineRenderer> m_renderer;
};
