#include <cstdlib>
#include <iostream>
#include <bifrost/core/window.h>

int main(void) {
    bifrost::core::Window window = bifrost::core::Window(
        640,
        480,
        "Test Window"
    );

    window.show();
    std::cout << "Window initialized\n";

    return EXIT_SUCCESS;
}
