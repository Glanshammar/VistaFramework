#include "ConsoleTest.hpp"
#include "PlatformTest.hpp"
#include "ObjectTest.hpp"
#include "ArrayTest.hpp"
#include "VThreadTest.hpp"
#include "VAnyTest.hpp"
#include <VApplication>
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <stdexcept>

using std::cout;
using std::string;
using std::vector;

void Help();

std::unordered_map<string, std::function<void()>> testMap = {
    {"console", ConsoleTest},
    {"platform", PlatformTest},
    {"object", ObjectTest},
    {"array", RunArrayTest},
    {"thread", VThreadTest},
    {"any", []() { VAnyTest(); }},
    {"help", Help}
};

void Help()
{
    cout << "Available tests:" << std::endl;
    for (const auto& [key, value] : testMap) {
        cout << "  " << key << std::endl;
    }
}

int main()
{
#if defined(_WIN32) || defined(_WIN64)
    VApplication::setIcon("C:/Users/Mondus/Pictures/Programs/mandala.ico");
    VApplication::setTitleBar("VistaCore Test Console");
#endif

    string input;

    while(true){
        cout << " >> " << std::flush;

        std::getline(std::cin, input);

        if(input == "exit" || input == "quit" || input == "q" || input == "e") {
            break;
        }

        if (input == "clear" || input == "cls") {
            system("cls");
            continue;
        }

        if (input.empty()) {
            continue;
        }

        auto it = testMap.find(input);
        if (it != testMap.end()) {
            try {
                it->second();  // Call the function stored in the map
            } catch (const std::exception& e) {
                cout << "\nError in test '" << input << "': " << e.what() << std::endl;
            } catch (...) {
                cout << "\nUnknown error occurred in test '" << input << "'" << std::endl;
            }
        } else {
            cout << "Invalid test name." << std::endl;
        }
    }
}
