#include <iostream>
#include <memory>

#include "PlainText.hpp"
#include "BoldDecorator.hpp"
#include "ItalicDecorator.hpp"
#include "UnderlineDecorator.hpp"
#include "ShadowDecorator.hpp"

int main() {
    std::unique_ptr<Text> text = std::make_unique<PlainText>("Hello Decorator!");

    // Client code stays the same; we just wrap with decorators.
    text = std::make_unique<BoldDecorator>(std::move(text));
    text = std::make_unique<ItalicDecorator>(std::move(text));
    text = std::make_unique<UnderlineDecorator>(std::move(text));
    text = std::make_unique<ShadowDecorator>(std::move(text)); // extra challenge

    std::cout << text->render() << "\n";
    return 0;
}
