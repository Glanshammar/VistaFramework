#include "ConsoleTest.hpp"
#include "PlatformTest.hpp"
#include "ObjectTest.hpp"
#include "ArrayTest.hpp"
#include <VApplication>
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>

using std::cout;
using std::string;
using std::vector;


std::unordered_map<string, std::function<void()>> testMap = {
    {"console", ConsoleTest},
    {"platform", PlatformTest},
    {"object", ObjectTest},
    {"array", ArrayTest},
    };

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
            // If found, execute the corresponding function
            it->second();  // Call the function stored in the map
        } else {
            cout << "Invalid test name." << std::endl;
        }
    }
}
