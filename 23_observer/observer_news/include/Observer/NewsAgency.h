#pragma once
#include <memory>
#include <string>
#include <vector>

class IObserver;

class NewsAgency {
public:
    void attach(const std::shared_ptr<IObserver>& observer);
    void detach(const std::shared_ptr<IObserver>& observer);

    void publishNews(const std::string& news);

private:
    void notify(const std::string& news);
    void cleanupExpired();

private:
    std::vector<std::weak_ptr<IObserver>> observers_;
};
