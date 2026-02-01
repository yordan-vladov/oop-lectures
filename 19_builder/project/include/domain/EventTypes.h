#pragma once

#include <string>

namespace domain {

enum class EventType { Webinar, Conference, Hybrid };
enum class AccessType { Free, Paid };

inline std::string toString(EventType t) {
    switch (t) {
        case EventType::Webinar: return "Webinar";
        case EventType::Conference: return "Conference";
        case EventType::Hybrid: return "Hybrid";
        default: return "Unknown";
    }
}

inline std::string toString(AccessType a) {
    switch (a) {
        case AccessType::Free: return "Free";
        case AccessType::Paid: return "Paid";
        default: return "Unknown";
    }
}

} // namespace domain
