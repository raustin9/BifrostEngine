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
    sleep(2);
    window.set_title("Window Name 2");
    sleep(2);
    
    window.shutdown();
    
    std::cout << "Window initialized\n";

    return EXIT_SUCCESS;
}
