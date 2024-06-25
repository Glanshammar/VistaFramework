#include "../GUI/VConsole.hpp"

int testInt = 5;

int main() {
    VConsole console;
    console.Print("Hello, World! Number: ", testInt, "\n", Color::RED, "This is a test, in red!");
}