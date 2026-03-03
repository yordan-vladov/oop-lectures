#include "app/Application.h"
#include "logging/LoggerFactory.h"

#include <iostream>

int main() {
    try {
        auto consoleLogger = makeConsoleLogger();
        Application app1(consoleLogger);
        app1.run();

        std::cout << "-----\n";

        auto fileLogger = makeFileLogger("app.log");
        Application app2(fileLogger);
        app2.run();

        std::cout << "Logs written to app.log\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
