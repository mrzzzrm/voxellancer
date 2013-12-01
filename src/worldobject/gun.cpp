#include "gun.h"

Gun::Gun(float speed, float range, float cooldown){
    m_speed = speed;
    m_range = range;
    m_cooldownTime = cooldown;
    m_cooldown = 0;
}

void Gun::update(float delta_sec){
    if (m_cooldown <= 0){ //avoid negative overflow
        m_cooldown = 0;
    }
    else{
        m_cooldown -= delta_sec;
    }
}


Bullet *Gun::shootAt(glm::vec3 position, glm::quat orientation, glm::vec3 point){
    if (m_cooldown <= 0){
        Bullet *b = new Bullet(position, orientation, point - position, m_speed, m_range);
     
        //b->transform().setOrientation(glm::quat_cast(glm::lookAt(position, direction, glm::vec3(0, -1, 0))));
        //b->angularSpeed(glm::vec3(10000, 10000, 10000)); //set spinning
        ClusterCache::instance()->fillCluster(b, "data/voxelcluster/bullet.csv");
        m_cooldown = m_cooldownTime;
        return b;
    }
    return NULL;
}

float Gun::range(){
    return m_range;
}