#include <simulation/window.hpp>

int main() {
    Window window(600, 400, "Movement interpolation");

    window.RunMessageLoop();

    return 0;
}
