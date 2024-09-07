#pragma once

#include <VObject>

void ObjectTest();

class VButton : public VObject {
private:
public:
    Signal<int, int> clicked;

    VButton();
    ~VButton();
    void click(int x, int y);
};

void onButtonClicked(int x, int y);
void anotherButtonClickedHandler(int x, int y);