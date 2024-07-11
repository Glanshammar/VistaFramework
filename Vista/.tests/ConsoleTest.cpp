#include <VString>
#include <VConsole>
#include "ConsoleTest.hpp"

#define vString std::u16string

void Print(const std::u16string& str){
    for (char16_t ch : str) {
        std::wcout << (wchar_t)ch;
    }
    std::cout << std::endl;
}

void ConsoleTest(){
    int testValue = 42;
    VString test = "VString test. 😊🌍";
    vString u16str = u"Hello World from u16string! 😊🌍";


    VDebug(Level::Info, "This is an informational message.");
    VDebug(Level::Warning, "Value might be out of range: " + std::to_string(testValue));
    VDebug(Level::Error, "Critical error occurred!");
    std::cout << test << std::endl;
    Print(u16str);
}