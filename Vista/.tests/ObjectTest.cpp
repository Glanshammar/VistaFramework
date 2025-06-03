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

void VButton::click(const int x, const int y) {
    VPrint("Button clicked at (", x, ", ", y, ")!");
    clicked.emit(x, y);
}

void onButtonClicked(const int x, const int y) {
    VPrint("Button was clicked at (", x, ", ", y, ") (slot 1)!");
}

void anotherButtonClickedHandler(const int x, const int y) {
    VPrint("Button was clicked at (", x, ", ", y, ") (slot 2)!");
}

void onButtonDestroyFunc(const VObject* button)
{
    VPrint("Button destroyed: ", button->getName());
}

void ObjectTest(){
    VPrint("\n=== Starting Object Test ===");
    
    {  // Scope block to control object lifetime
        VApplication app;
        app.setName("Console Application");
        VPrint("Created application: ", app.getName());

        VPrint("\nCreating buttons...");
        VButton button;
        button.setName("Button 1");
        button.setParent(&app);
        VPrint("Created ", button.getName());

        VButton button2;
        button2.setName("Button 2");
        button2.setParent(&app);
        VPrint("Created ", button2.getName());

        VButton button3;
        button3.setName("Button 3");
        button3.setParent(&app);
        VPrint("Created ", button3.getName());

        VPrint("\nSetting up button handlers...");
        button.onDestroyed(onButtonDestroyFunc);
        button2.onDestroyed(onButtonDestroyFunc);
        button3.onDestroyed(onButtonDestroyFunc);

        button.clicked.connect(onButtonClicked);
        button.clicked.connect(anotherButtonClickedHandler);
        
        VPrint("\nTesting button click...");
        button.click(10, 20);

        VPrint("\nTesting parent-child relationships...");
        VPrint("Button 1 memory: ", &button);
        VPrint("Button 1 parent: ", button.getParent()->getName());
        VPrint("Button 1 parent memory: ", button.getParent());
        VPrint("App memory: ", &app);

        VPrint("\nAttempting to set button as its own parent...");
        button.setParent(&button);
        
        VPrint("\n=== Main Test Complete ===");
        VPrint("\nObjects will now be destroyed. Watch for destruction messages:");
        VPrint("-------------------------------------------------------------");
    }  // Objects will be destroyed here
    
    VPrint("\n=== Object Test Complete ===\n");
}