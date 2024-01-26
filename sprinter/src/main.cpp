#include <iostream>

#include "sprinter.hpp"

int main(int argc, char *argv[]) {
    // instance main application
    std::cout << "start of program" << std::endl;
    Sprinter* app = new Sprinter();
    app->Run();
    delete app;
    std::cout << "end of program" << std::endl;
    return 0;
}
