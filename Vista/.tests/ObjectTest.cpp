#include "ObjectTest.hpp"

static int32_t buttonCount = 0;

Button::Button() {
    buttonID = ++buttonCount;
}

Button::~Button() {
    buttonCount--;
}

void Button::click(int x, int y) {
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
    Button button;
    button.clicked.connect(onButtonClicked);
    button.clicked.connect(anotherButtonClickedHandler);

    button.click(10, 20);
}