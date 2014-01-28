#include "targetguide.h"

#include <algorithm>

#include "utils/geometryhelper.h"

#include "worldobject/ship.h"
#include "worldobject/worldobject.h"

#include "player.h"

#include "hud.h"
#include "targetguidearrow.h"


static float ARROWS_PER_RAD = 5.0f;

TargetGuide::TargetGuide(HUD* hud):
    m_hud(hud)
{
}

HUD* TargetGuide::hud() {
    return m_hud;
}

void TargetGuide::update(float deltaSec) {
    for(TargetGuideArrow* arrow : m_arrows) {
        arrow->setVisible(target() != nullptr);
    }

    if(target()) {
        adjustArrowCount();
        adjustArrows();

        for(TargetGuideArrow* arrow : m_arrows) {
            arrow->update(deltaSec);
        }
    }
}

void TargetGuide::draw() {
    for(TargetGuideArrow* arrow : m_arrows) {
        arrow->draw();
    }
}

WorldObject* TargetGuide::target() {
    Ship* playerShip = m_hud->player()->playerShip();
    return playerShip ? playerShip->targetObject() : nullptr;
}

glm::quat TargetGuide::orientationToTarget() {
    glm::vec3 delta = target()->transform().position() - m_hud->position();
    glm::vec3 relativeDelta = glm::inverse(m_hud->orientation()) * delta;
    return GeometryHelper::quatFromViewDirection(relativeDelta);
}

void TargetGuide::adjustArrowCount() {
    int arrowCount = glm::radians(glm::angle(glm::inverse(m_hud->crossHair().orientationOffset()) * orientationToTarget())) * ARROWS_PER_RAD;

    if(m_arrows.size() < arrowCount) {
        for(int i = m_arrows.size(); i < arrowCount; i++) {
            TargetGuideArrow* arrow = new TargetGuideArrow(this);
            m_arrows.push_back(arrow);
            m_hud->addHudget(arrow);
        }
    } else {
        for(int i = arrowCount; i < m_arrows.size(); i++) {
            m_hud->removeHudget(m_arrows[i]);
            delete m_arrows[i];
        }
        m_arrows.resize(arrowCount);
    }
}

void TargetGuide::adjustArrows() {
    glm::quat targetOrientation = orientationToTarget();

    for(int i = 0; i < m_arrows.size(); i++) {
        TargetGuideArrow* arrow = m_arrows[i];

        arrow->setTargetOrientation(targetOrientation);

        glm::quat orientation = glm::slerp(m_hud->crossHair().orientationOffset(), targetOrientation, static_cast<float>(i+1) / (m_arrows.size() + 1));
        arrow->setRelativeOrientation(orientation);
    }
}

