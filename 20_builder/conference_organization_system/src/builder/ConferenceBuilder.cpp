#include "builder/ConferenceBuilder.h"
#include <stdexcept>

namespace builder {

ConferenceBuilder::ConferenceBuilder() { reset(); }

void ConferenceBuilder::reset() {
    m_access = domain::AccessType::Free;
    m_title.clear();
    m_dateTimeIso.clear();
    m_durationMinutes = 0;
    m_speakers.clear();
    m_maxParticipants = 0;
    m_price = 0.0;
    m_location.clear();
}

void ConferenceBuilder::setTitle(const std::string& title) { m_title = title; }
void ConferenceBuilder::setDateTimeIso(const std::string& dateTimeIso) { m_dateTimeIso = dateTimeIso; }
void ConferenceBuilder::setDurationMinutes(int minutes) { m_durationMinutes = minutes; }
void ConferenceBuilder::setSpeakers(const std::vector<std::string>& speakers) { m_speakers = speakers; }
void ConferenceBuilder::setMaxParticipants(int max) { m_maxParticipants = max; }

void ConferenceBuilder::setAccessFree() {
    m_access = domain::AccessType::Free;
    m_price = 0.0;
}

void ConferenceBuilder::setAccessPaid(double price) {
    m_access = domain::AccessType::Paid;
    m_price = price;
}

void ConferenceBuilder::setLocation(const std::string& location) { m_location = location; }

void ConferenceBuilder::setOnlineLink(const std::string&) {
    // Conference няма онлайн линк (умишлено игнорираме)
}

std::unique_ptr<domain::Event> ConferenceBuilder::build() {
    if (m_location.empty())
        throw std::invalid_argument("ConferenceBuilder: location is required.");

    auto ev = std::make_unique<domain::Event>(
        domain::EventType::Conference,
        m_access,
        m_title,
        m_dateTimeIso,
        m_durationMinutes,
        m_speakers,
        m_maxParticipants,
        m_price,
        /*location*/ m_location,
        /*onlineLink*/ ""
    );

    reset();
    return ev;
}

} // namespace builder
