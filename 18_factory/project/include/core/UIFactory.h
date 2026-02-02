#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class UIElement;

class UIFactory {
public:
    using Creator = std::function<std::unique_ptr<UIElement>()>;

    static std::unique_ptr<UIElement> createElement(const std::string& type);
    static bool registerType(const std::string& type, Creator creator);
    static std::vector<std::string> registeredTypes();

private:
    static std::unordered_map<std::string, Creator>& registry();
};
