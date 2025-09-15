#include <iostream>
#include "la_extended.h"
using namespace LA;

void test_accept_float_ptr(float* ptr) {
    return;
}

int main() {
    mat4 m = mat4();
    std::cout << m << std::endl;
    std::cout << &m << std::endl;
    std::cout << &m[0][0] << std::endl;
    test_accept_float_ptr((float*)&m);
    // std::cout << &m.data << std::endl;
    // std::cout << &m.front() << std::endl;
    return 0;
}