#pragma once

#include "builder/EventBuilder.h"
#include <memory>

namespace builder {

class EventDirector {
public:
    std::unique_ptr<domain::Event> makeStandardFreeWebinar(EventBuilder& b) const;
    std::unique_ptr<domain::Event> makePaidConference(EventBuilder& b) const;
    std::unique_ptr<domain::Event> makeCorporateHybrid(EventBuilder& b) const;
};

} // namespace builder
