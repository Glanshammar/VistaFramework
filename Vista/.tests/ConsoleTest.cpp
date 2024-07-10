#include <VConsole>
#include <VString>
#include "ConsoleTest.hpp"


void ConsoleTest(){
    int testValue = 42;
    VString test = "VString test.";

    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: " + std::to_string(testValue));
    VDebug(Level::Error, "Critical error occurred!");
    VPrint("Hello world! ", testValue, " Yo mama!");
    VPrint("Bye world! ", testValue * 2, " Yo papa!");
    VPrint(test);
}