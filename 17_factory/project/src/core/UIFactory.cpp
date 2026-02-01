#include "core/UIFactory.h"
#include "core/UIElement.h"

#include <algorithm>

std::unordered_map<std::string, UIFactory::Creator>& UIFactory::registry() {
    static std::unordered_map<std::string, Creator> instance;
    return instance;
}

bool UIFactory::registerType(const std::string& type, Creator creator) {
    auto& reg = registry();
    auto [it, inserted] = reg.emplace(type, std::move(creator));
    return inserted;
}

std::unique_ptr<UIElement> UIFactory::createElement(const std::string& type) {
    auto& reg = registry();
    auto it = reg.find(type);
    if (it == reg.end()) return nullptr;
    return (it->second)();
}

std::vector<std::string> UIFactory::registeredTypes() {
    std::vector<std::string> keys;
    keys.reserve(registry().size());
    for (const auto& kv : registry()) keys.push_back(kv.first);
    std::sort(keys.begin(), keys.end());
    return keys;
}
