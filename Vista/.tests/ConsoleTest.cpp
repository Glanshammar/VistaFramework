#include <VString>
#include <VConsole>
#include <VThread>
#include <VDefines>
#include "ConsoleTest.hpp"

int testValue = 42;

void ThreadFunction()
{
    testValue++;
}

void ConsoleTest(){
    VString testString = "Hello, World!";

    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: " + std::to_string(testValue));
    VDebug(Level::Error, "Critical error occurred!");
    VPrint(testString);

    VThreadGroup threadGroup;

    for (Range(100))
    {
        threadGroup.addThread(VThread(ThreadFunction));
    }

    threadGroup.startAll();
    threadGroup.joinAll();

    VPrint(std::to_string(testValue));
}