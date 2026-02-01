#include "domain/Event.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace domain {

Event::Event(EventType type,
             AccessType access,
             std::string title,
             std::string dateTimeIso,
             int durationMinutes,
             std::vector<std::string> speakers,
             int maxParticipants,
             double price,
             std::string location,
             std::string onlineLink)
    : m_type(type),
      m_access(access),
      m_title(std::move(title)),
      m_dateTimeIso(std::move(dateTimeIso)),
      m_durationMinutes(durationMinutes),
      m_speakers(std::move(speakers)),
      m_maxParticipants(maxParticipants),
      m_price(price),
      m_location(std::move(location)),
      m_onlineLink(std::move(onlineLink))
{
    if (m_title.empty()) throw std::invalid_argument("Event title cannot be empty.");
    if (m_durationMinutes <= 0) throw std::invalid_argument("Duration must be positive.");
    if (m_maxParticipants < 0) throw std::invalid_argument("Max participants cannot be negative.");

    // Минимална консистентност според типа
    if ((m_type == EventType::Webinar || m_type == EventType::Hybrid) && m_onlineLink.empty())
        throw std::invalid_argument("Online link is required for Webinar/Hybrid.");
    if ((m_type == EventType::Conference || m_type == EventType::Hybrid) && m_location.empty())
        throw std::invalid_argument("Location is required for Conference/Hybrid.");

    // Access rules
    if (m_access == AccessType::Free && m_price != 0.0)
        throw std::invalid_argument("Free event must have price 0.");
    if (m_access == AccessType::Paid && m_price <= 0.0)
        throw std::invalid_argument("Paid event must have positive price.");
}

double Event::totalCost() const {
    // Примерна формула (можете да я смените според задачата)
    // Base = price (ако Paid), иначе 0
    // Add: venue fee за Conference/Hybrid, platform fee за Webinar/Hybrid
    double cost = (m_access == AccessType::Paid) ? m_price : 0.0;

    const double venueFee = (m_type == EventType::Conference || m_type == EventType::Hybrid) ? 150.0 : 0.0;
    const double platformFee = (m_type == EventType::Webinar || m_type == EventType::Hybrid) ? 40.0 : 0.0;

    return cost + venueFee + platformFee;
}

void Event::print() const {
    std::cout << "=== Event ===\n";
    std::cout << "Type: " << toString(m_type) << "\n";
    std::cout << "Access: " << toString(m_access) << "\n";
    std::cout << "Title: " << m_title << "\n";
    std::cout << "Date/Time: " << m_dateTimeIso << "\n";
    std::cout << "Duration: " << m_durationMinutes << " min\n";
    std::cout << "Max participants: " << m_maxParticipants << "\n";

    std::cout << "Speakers: ";
    if (m_speakers.empty()) std::cout << "(none)";
    for (size_t i = 0; i < m_speakers.size(); ++i) {
        std::cout << m_speakers[i];
        if (i + 1 < m_speakers.size()) std::cout << ", ";
    }
    std::cout << "\n";

    if (!m_location.empty())
        std::cout << "Location: " << m_location << "\n";
    if (!m_onlineLink.empty())
        std::cout << "Online link: " << m_onlineLink << "\n";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Price: " << m_price << "\n";
    std::cout << "Total cost: " << totalCost() << "\n";
    std::cout << "============\n\n";
}

} // namespace domain
