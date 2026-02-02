#pragma once

#include "domain/Event.h"
#include <memory>
#include <string>
#include <vector>

namespace builder {

class EventFluentBuilder {
public:
    EventFluentBuilder();

    EventFluentBuilder& type(domain::EventType t);
    EventFluentBuilder& accessFree();
    EventFluentBuilder& accessPaid(double price);

    EventFluentBuilder& title(std::string t);
    EventFluentBuilder& dateTimeIso(std::string dt);
    EventFluentBuilder& durationMinutes(int m);
    EventFluentBuilder& speakers(std::vector<std::string> s);
    EventFluentBuilder& maxParticipants(int max);

    EventFluentBuilder& location(std::string loc);
    EventFluentBuilder& onlineLink(std::string link);

    std::unique_ptr<domain::Event> build() const;

private:
    domain::EventType m_type = domain::EventType::Webinar;
    domain::AccessType m_access = domain::AccessType::Free;

    std::string m_title;
    std::string m_dateTimeIso;
    int m_durationMinutes = 0;
    std::vector<std::string> m_speakers;
    int m_maxParticipants = 0;

    double m_price = 0.0;
    std::string m_location;
    std::string m_onlineLink;
};

} // namespace builder
