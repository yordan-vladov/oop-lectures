#pragma once

#include "domain/EventTypes.h"
#include <string>
#include <vector>

namespace domain {

class Event {
public:
    Event(EventType type,
          AccessType access,
          std::string title,
          std::string dateTimeIso,
          int durationMinutes,
          std::vector<std::string> speakers,
          int maxParticipants,
          double price,
          std::string location,
          std::string onlineLink);

    // Getters
    EventType type() const noexcept { return m_type; }
    AccessType access() const noexcept { return m_access; }
    const std::string& title() const noexcept { return m_title; }
    const std::string& dateTimeIso() const noexcept { return m_dateTimeIso; }
    int durationMinutes() const noexcept { return m_durationMinutes; }
    const std::vector<std::string>& speakers() const noexcept { return m_speakers; }
    int maxParticipants() const noexcept { return m_maxParticipants; }
    double price() const noexcept { return m_price; }
    const std::string& location() const noexcept { return m_location; }
    const std::string& onlineLink() const noexcept { return m_onlineLink; }

    // Business helper
    double totalCost() const;      // примерна бизнес логика
    void print() const;            // удобен изход за демонстрация

private:
    EventType m_type;
    AccessType m_access;

    std::string m_title;
    std::string m_dateTimeIso;     // ISO-like string: "2026-02-10 18:30"
    int m_durationMinutes;

    std::vector<std::string> m_speakers;
    int m_maxParticipants;

    double m_price;                // само ако Paid
    std::string m_location;        // за Conference/Hybrid
    std::string m_onlineLink;      // за Webinar/Hybrid
};

} // namespace domain
