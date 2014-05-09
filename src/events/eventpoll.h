#pragma once

#include <memory>

#include "utils/callback.h"

#include "world/worldelement.h"


class EventPoll: public WorldElement {
public:
    EventPoll(const Callback& callback);
    virtual ~EventPoll() override;

    virtual void update(float deltaSec) override;

    bool isActive() const;
    void setActive(bool active);


protected:
    Callback m_callback;
    bool m_active;

    void doCallback();
    virtual bool poll() = 0;
    virtual void specialOnCallback();
};

