#include "builder/WebinarBuilder.h"
#include "builder/ConferenceBuilder.h"
#include "builder/HybridEventBuilder.h"
#include "builder/EventDirector.h"
#include "builder/EventFluentBuilder.h"

#include <iostream>

int main() {
    try {
        builder::EventDirector director;

        // Director + конкретен Builder: стандартен безплатен уебинар
        builder::WebinarBuilder webinarBuilder;
        auto webinar = director.makeStandardFreeWebinar(webinarBuilder);
        webinar->print();

        // Director + конкретен Builder: платена конференция
        builder::ConferenceBuilder confBuilder;
        auto conference = director.makePaidConference(confBuilder);
        conference->print();

        // Director + конкретен Builder: корпоративно хибридно събитие
        builder::HybridEventBuilder hybridBuilder;
        auto hybrid = director.makeCorporateHybrid(hybridBuilder);
        hybrid->print();

        // Fluent Builder: персонализирано събитие
        auto custom = builder::EventFluentBuilder()
            .type(domain::EventType::Webinar)
            .accessPaid(35.0)
            .title("Advanced Builder Pattern Workshop")
            .dateTimeIso("2026-02-15 19:00")
            .durationMinutes(120)
            .speakers({"Trainer A", "Trainer B"})
            .maxParticipants(150)
            .onlineLink("https://meet.example.com/advanced-builder")
            .build();

        custom->print();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
