#include "builder/WebinarBuilder.h"
#include <stdexcept>

namespace builder {

WebinarBuilder::WebinarBuilder() { reset(); }

void WebinarBuilder::reset() {
    m_access = domain::AccessType::Free;
    m_title.clear();
    m_dateTimeIso.clear();
    m_durationMinutes = 0;
    m_speakers.clear();
    m_maxParticipants = 0;
    m_price = 0.0;
    m_onlineLink.clear();
}

void WebinarBuilder::setTitle(const std::string& title) { m_title = title; }
void WebinarBuilder::setDateTimeIso(const std::string& dateTimeIso) { m_dateTimeIso = dateTimeIso; }
void WebinarBuilder::setDurationMinutes(int minutes) { m_durationMinutes = minutes; }
void WebinarBuilder::setSpeakers(const std::vector<std::string>& speakers) { m_speakers = speakers; }
void WebinarBuilder::setMaxParticipants(int max) { m_maxParticipants = max; }

void WebinarBuilder::setAccessFree() {
    m_access = domain::AccessType::Free;
    m_price = 0.0;
}

void WebinarBuilder::setAccessPaid(double price) {
    m_access = domain::AccessType::Paid;
    m_price = price;
}

void WebinarBuilder::setLocation(const std::string&) {
    // Webinar няма локация (умишлено игнорираме)
}

void WebinarBuilder::setOnlineLink(const std::string& link) { m_onlineLink = link; }

std::unique_ptr<domain::Event> WebinarBuilder::build() {
    if (m_onlineLink.empty())
        throw std::invalid_argument("WebinarBuilder: online link is required.");

    auto ev = std::make_unique<domain::Event>(
        domain::EventType::Webinar,
        m_access,
        m_title,
        m_dateTimeIso,
        m_durationMinutes,
        m_speakers,
        m_maxParticipants,
        m_price,
        /*location*/ "",
        /*onlineLink*/ m_onlineLink
    );

    reset();
    return ev;
}

} // namespace builder
