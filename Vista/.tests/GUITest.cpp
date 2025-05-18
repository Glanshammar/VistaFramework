#include <VApplication>

int main() {
    X11Window window;
    if (window.init()) {
        window.run();
    }
    return 0;
}