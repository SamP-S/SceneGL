#include <iostream>
#include <stdexcept>


char infoLog[1024] = "test char array";

void error() {
    throw infoLog;
    throw std::runtime_error("test error");
    throw ("big fat error message");
}

void dummy() {
    error();
}

int main() {
    try {
        dummy();
    } catch (const char* msg) {
        std::cout << "caught string error" << std::endl;
        std::cout << msg << std::endl;
    } catch (std::runtime_error err) {
        std::cout << "caught runtime error" << std::endl;
        std::cout << err.what() << std::endl;
    } 
    
    return 0;
}