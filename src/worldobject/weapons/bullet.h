#include "worldobject/worldobject.h"
#include "resource/clustercache.h"

class Bullet : public WorldObject {

public:
    Bullet(glm::vec3 position, glm::quat orientation, glm::vec3 direction, float speed, float range);
    virtual void update(float delta_sec);

    virtual void onCollision() override;
    virtual void onSpawnFail() override;

private:
	float m_lifetime;
};
