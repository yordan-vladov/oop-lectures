#pragma once

#include "domain/Event.h"
#include <memory>
#include <string>
#include <vector>

namespace builder {

class EventBuilder {
public:
    virtual ~EventBuilder() = default;

    // Reset state for a new build
    virtual void reset() = 0;

    // Steps
    virtual void setTitle(const std::string& title) = 0;
    virtual void setDateTimeIso(const std::string& dateTimeIso) = 0;
    virtual void setDurationMinutes(int minutes) = 0;
    virtual void setSpeakers(const std::vector<std::string>& speakers) = 0;
    virtual void setMaxParticipants(int max) = 0;

    virtual void setAccessFree() = 0;
    virtual void setAccessPaid(double price) = 0;

    virtual void setLocation(const std::string& location) = 0;
    virtual void setOnlineLink(const std::string& link) = 0;

    // Finish
    virtual std::unique_ptr<domain::Event> build() = 0;
};

} // namespace builder
