#include "builder/HybridEventBuilder.h"
#include <stdexcept>

namespace builder {

HybridEventBuilder::HybridEventBuilder() { reset(); }

void HybridEventBuilder::reset() {
    m_access = domain::AccessType::Free;
    m_title.clear();
    m_dateTimeIso.clear();
    m_durationMinutes = 0;
    m_speakers.clear();
    m_maxParticipants = 0;
    m_price = 0.0;
    m_location.clear();
    m_onlineLink.clear();
}

void HybridEventBuilder::setTitle(const std::string& title) { m_title = title; }
void HybridEventBuilder::setDateTimeIso(const std::string& dateTimeIso) { m_dateTimeIso = dateTimeIso; }
void HybridEventBuilder::setDurationMinutes(int minutes) { m_durationMinutes = minutes; }
void HybridEventBuilder::setSpeakers(const std::vector<std::string>& speakers) { m_speakers = speakers; }
void HybridEventBuilder::setMaxParticipants(int max) { m_maxParticipants = max; }

void HybridEventBuilder::setAccessFree() {
    m_access = domain::AccessType::Free;
    m_price = 0.0;
}

void HybridEventBuilder::setAccessPaid(double price) {
    m_access = domain::AccessType::Paid;
    m_price = price;
}

void HybridEventBuilder::setLocation(const std::string& location) { m_location = location; }
void HybridEventBuilder::setOnlineLink(const std::string& link) { m_onlineLink = link; }

std::unique_ptr<domain::Event> HybridEventBuilder::build() {
    if (m_location.empty())
        throw std::invalid_argument("HybridEventBuilder: location is required.");
    if (m_onlineLink.empty())
        throw std::invalid_argument("HybridEventBuilder: online link is required.");

    auto ev = std::make_unique<domain::Event>(
        domain::EventType::Hybrid,
        m_access,
        m_title,
        m_dateTimeIso,
        m_durationMinutes,
        m_speakers,
        m_maxParticipants,
        m_price,
        /*location*/ m_location,
        /*onlineLink*/ m_onlineLink
    );

    reset();
    return ev;
}

} // namespace builder
