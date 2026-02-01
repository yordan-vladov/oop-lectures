#include "builder/EventFluentBuilder.h"
#include <utility>

namespace builder {

EventFluentBuilder::EventFluentBuilder() = default;

EventFluentBuilder& EventFluentBuilder::type(domain::EventType t) {
    m_type = t;
    return *this;
}

EventFluentBuilder& EventFluentBuilder::accessFree() {
    m_access = domain::AccessType::Free;
    m_price = 0.0;
    return *this;
}

EventFluentBuilder& EventFluentBuilder::accessPaid(double price) {
    m_access = domain::AccessType::Paid;
    m_price = price;
    return *this;
}

EventFluentBuilder& EventFluentBuilder::title(std::string t) {
    m_title = std::move(t);
    return *this;
}

EventFluentBuilder& EventFluentBuilder::dateTimeIso(std::string dt) {
    m_dateTimeIso = std::move(dt);
    return *this;
}

EventFluentBuilder& EventFluentBuilder::durationMinutes(int m) {
    m_durationMinutes = m;
    return *this;
}

EventFluentBuilder& EventFluentBuilder::speakers(std::vector<std::string> s) {
    m_speakers = std::move(s);
    return *this;
}

EventFluentBuilder& EventFluentBuilder::maxParticipants(int max) {
    m_maxParticipants = max;
    return *this;
}

EventFluentBuilder& EventFluentBuilder::location(std::string loc) {
    m_location = std::move(loc);
    return *this;
}

EventFluentBuilder& EventFluentBuilder::onlineLink(std::string link) {
    m_onlineLink = std::move(link);
    return *this;
}

std::unique_ptr<domain::Event> EventFluentBuilder::build() const {
    return std::make_unique<domain::Event>(
        m_type,
        m_access,
        m_title,
        m_dateTimeIso,
        m_durationMinutes,
        m_speakers,
        m_maxParticipants,
        m_price,
        m_location,
        m_onlineLink
    );
}

} // namespace builder
