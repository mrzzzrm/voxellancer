#include "patrolwaypointstask.h"

#include "worldobject/ship.h"
#include "ai/formationlogic.h"


PatrolWaypointsTask::PatrolWaypointsTask(Ship& ship, std::list<glm::vec3> points) :
    AiTask(ship.boardComputer()),
    m_ship(ship),
    m_points(points),
    m_currentPoint(m_points.begin()),
    m_flyTask(ship.boardComputer()),
    m_formationTask(ship)
{
}

void PatrolWaypointsTask::update(float deltaSec) {
    if (m_ship.formationLogic()->inFormation()) {
        m_formationTask.update(deltaSec);
    } else {
        float distance = glm::length(*m_currentPoint - m_ship.transform().position());
        if (distance < m_ship.bounds().sphere().radius()){
            m_currentPoint++;
            if (m_currentPoint == m_points.end()) {
                m_currentPoint = m_points.begin();
            }
        }
        m_flyTask.setTargetPoint(*m_currentPoint);
        m_flyTask.update(deltaSec);
    }
}

