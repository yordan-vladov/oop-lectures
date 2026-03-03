#include "Observer/NewsAgency.h"
#include "Observer/IObserver.h"

#include <algorithm>

void NewsAgency::attach(const std::shared_ptr<IObserver>& observer) {
    if (!observer) return;
    observers_.push_back(observer);
}

void NewsAgency::detach(const std::shared_ptr<IObserver>& observer) {
    if (!observer) return;

    observers_.erase(
        std::remove_if(observers_.begin(), observers_.end(),
            [&](const std::weak_ptr<IObserver>& w) {
                auto s = w.lock();
                return !s || s == observer;
            }),
        observers_.end()
    );
}

void NewsAgency::publishNews(const std::string& news) {
    notify(news);
    cleanupExpired();
}

void NewsAgency::notify(const std::string& news) {
    for (auto& w : observers_) {
        if (auto obs = w.lock()) {
            obs->update(news);
        }
    }
}

void NewsAgency::cleanupExpired() {
    observers_.erase(
        std::remove_if(observers_.begin(), observers_.end(),
            [](const std::weak_ptr<IObserver>& w) { return w.expired(); }),
        observers_.end()
    );
}
