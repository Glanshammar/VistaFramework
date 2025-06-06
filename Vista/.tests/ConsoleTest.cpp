#include "ConsoleTest.hpp"
#include <VistaCore>
#include <VConsole>
#include <VGlobals>

void ConsoleTest(){
    VString testString = "Hello 👋 World 🌍";

    VPrint(std::flush);
    VPrint("Hello\nWorld", std::endl);
    VPrint("Hello", "World", std::endl);
    VPrint("Hello", "World", std::endl);
    VPrint("First line\n", "Second line\n", "Third line", std::endl);

    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: 50");
    VDebug(Level::Error, "Critical error occurred!");
    VPrint(testString, std::endl);
}