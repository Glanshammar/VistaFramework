#include "../GUI/VConsole.hpp"


int main() {
    int testValue = 42;

    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: " + std::to_string(testValue));
    VDebug(Level::Error, "Critical error occurred!");
    VPrint("Hello world! ", testValue, " Yo mama");
    std::cin.ignore();
}