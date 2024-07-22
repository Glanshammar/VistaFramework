#include "ObjectTest.hpp"
#include <VApplication>
#include <VistaCore>

static int32_t buttonCount = 0;

VButton::VButton() {
    buttonID = ++buttonCount;
}

VButton::~VButton() {
    buttonCount--;
}

void VButton::click(int x, int y) {
    std::cout << "Button clicked at (" << x << ", " << y << ")!\n";
    clicked.emit(x, y);
}

void onButtonClicked(int x, int y) {
    std::cout << "Button was clicked at (" << x << ", " << y << ") (slot 1)!\n";
}

void anotherButtonClickedHandler(int x, int y) {
    std::cout << "Button was clicked at (" << x << ", " << y << ") (slot 2)!\n";
}

void ObjectTest(){
    VApplication app;
    app.setName("Console Application");

    VButton button;
    button.setParent(&app);

    VButton button2;
    button2.setParent(&app);

    VButton button3;
    button3.setParent(&app);

    std::cout << "App name: " << app.getName() << std::endl;
    app.printChildren();

    button.clicked.connect(onButtonClicked);
    button.clicked.connect(anotherButtonClickedHandler);
    button.click(10, 20);
}