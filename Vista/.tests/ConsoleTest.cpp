#include "ConsoleTest.hpp"
#include <VistaCore>
#include <VConsole>
#include <VGlobals>

void ConsoleTest(){
    VString testString = "Hello 👋 World 🌍";

    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: 50");
    VDebug(Level::Error, "Critical error occurred!");
    VPrint(testString);
}