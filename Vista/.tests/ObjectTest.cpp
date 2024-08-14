#include "ObjectTest.hpp"
#include <VApplication>
#include <VistaCore>

static int32_t buttonCount = 0;


VButton::VButton() {
    setName("Button" + std::to_string(++buttonCount));
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

void onButtonDestroyFunc(const VObject* button)
{
    std::cout << "Button destroyed: " << button->getName() << std::endl;
}

void ObjectTest(){
    VApplication app;
    app.setName("Console Application");

    VButton button;
    button.setName("Button 1");
    button.setParent(&app);

    VButton button2;
    button2.setName("Button 2");
    button2.setParent(&app);

    VButton button3;
    button3.setName("Button 3");
    button3.setParent(&app);

    button.onDestroyed(onButtonDestroyFunc);
    button2.onDestroyed(onButtonDestroyFunc);
    button3.onDestroyed(onButtonDestroyFunc);


    button.clicked.connect(onButtonClicked);
    button.clicked.connect(anotherButtonClickedHandler);
    button.click(10, 20);

    button.setParent(&button);
    std:: cout << "Button 1 memory: " << &button << std::endl;
    std:: cout << "Button 1 parent: " << button.getParent()->getName() << std::endl;
    std:: cout << "App memory: " << &app << std::endl;
}