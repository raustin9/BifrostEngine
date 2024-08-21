#include <cstdlib>
#include <iostream>
#include <bifrost/core/core.h>
#include <unistd.h>

int main(void) {
    bifrost::core::Window window = bifrost::core::Window(
        640,
        480,
        "Test Window"
    );

    window.show();

    sleep(10);
    window.shutdown();
    
    std::cout << "Window initialized\n";

    return EXIT_SUCCESS;
}
