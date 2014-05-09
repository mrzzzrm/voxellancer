#pragma once

#include <glow/ref_ptr.h>

#include "missions/mission.h"

#include "utils/handle/handle.h"

#include "eventpoll.h"


class MissionStatePoll : public EventPoll {
public:
    MissionStatePoll(Mission& mission, MissionState state, const Callback& callback);


protected:
    const glow::ref_ptr<Mission> m_mission;
    MissionState m_state;
    bool m_fired;


    virtual bool poll() override;
    virtual void specialOnCallback() override;
};

