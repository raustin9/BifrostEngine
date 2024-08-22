#include <cstdlib>
#include <iostream>
#include <bifrost/core/core.h>
#include <unistd.h>

int main(void) {
    bifrost::core::Window window = bifrost::core::Window(
        640,
        480,
        "Window Name 1"
    );

    window.show();

    while (!window.should_close()) {

    }
    
    window.shutdown();

    return EXIT_SUCCESS;
}
