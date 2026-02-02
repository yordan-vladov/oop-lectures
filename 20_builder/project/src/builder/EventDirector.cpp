#include "builder/EventDirector.h"

namespace builder {

std::unique_ptr<domain::Event> EventDirector::makeStandardFreeWebinar(EventBuilder& b) const {
    b.reset();
    b.setTitle("Intro to C++ OOP");
    b.setDateTimeIso("2026-02-10 18:30");
    b.setDurationMinutes(60);
    b.setSpeakers({"Ivan Petrov"});
    b.setMaxParticipants(200);
    b.setAccessFree();
    b.setOnlineLink("https://meet.example.com/intro-cpp");
    return b.build();
}

std::unique_ptr<domain::Event> EventDirector::makePaidConference(EventBuilder& b) const {
    b.reset();
    b.setTitle("Software Architecture Conference");
    b.setDateTimeIso("2026-03-05 09:00");
    b.setDurationMinutes(480);
    b.setSpeakers({"Maria Ivanova", "Georgi Dimitrov"});
    b.setMaxParticipants(500);
    b.setAccessPaid(120.0);
    b.setLocation("Sofia Tech Park");
    return b.build();
}

std::unique_ptr<domain::Event> EventDirector::makeCorporateHybrid(EventBuilder& b) const {
    b.reset();
    b.setTitle("Corporate Town Hall");
    b.setDateTimeIso("2026-02-20 16:00");
    b.setDurationMinutes(90);
    b.setSpeakers({"CEO", "CTO"});
    b.setMaxParticipants(1000);
    b.setAccessFree();
    b.setLocation("HQ Auditorium");
    b.setOnlineLink("https://stream.example.com/townhall");
    return b.build();
}

} // namespace builder
