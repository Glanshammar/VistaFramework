#pragma once

#include <VObject>

void ObjectTest();

class Button {
private:
    int32_t buttonID;
public:
    Signal<int, int> clicked;

    Button();
    ~Button();
    void click(int x, int y);
};

void onButtonClicked(int x, int y);
void anotherButtonClickedHandler(int x, int y);