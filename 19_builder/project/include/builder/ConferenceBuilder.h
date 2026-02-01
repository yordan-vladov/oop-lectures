#pragma once

#include "builder/EventBuilder.h"
#include "domain/EventTypes.h"

namespace builder {

class ConferenceBuilder final : public EventBuilder {
public:
    ConferenceBuilder();

    void reset() override;

    void setTitle(const std::string& title) override;
    void setDateTimeIso(const std::string& dateTimeIso) override;
    void setDurationMinutes(int minutes) override;
    void setSpeakers(const std::vector<std::string>& speakers) override;
    void setMaxParticipants(int max) override;

    void setAccessFree() override;
    void setAccessPaid(double price) override;

    void setLocation(const std::string& location) override;
    void setOnlineLink(const std::string& link) override; // ignored / not required

    std::unique_ptr<domain::Event> build() override;

private:
    domain::AccessType m_access = domain::AccessType::Free;
    std::string m_title;
    std::string m_dateTimeIso;
    int m_durationMinutes = 0;
    std::vector<std::string> m_speakers;
    int m_maxParticipants = 0;
    double m_price = 0.0;
    std::string m_location;
};

} // namespace builder
