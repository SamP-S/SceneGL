#include <iostream>
#include <stdexcept>

#define LA_OPEN_GL
#include "la.h"
using namespace LA;

int main(int argc, char *argv[]) {
    vec2 v0 = vec2() = {1.0f, 3.0f};
    vec2 v1 = vec2() = {2.0f, 4.0f};

    mat2 m = mat2() = {v0, v1};

    mat<2, 1, float> v = { {10.0f}, {20.0f} };
    mat<2, 4, float> vt = {
        {10.0f, 20.0f, 30.0f, 40.0f},
        {50.0f, 60.0f, 70.0f, 80.0f}
     };

    mat<2, 2, float> mt = { 
        {30.0f, 40.0f}, 
        {50.0f, 60.0f}  
    };

    mat<2, 1, float> res = m * v;
    print(mt);
    print(v);
    print(res);

    std::cout << sizeof(m[0]) << std::endl;
    std::cout << sizeof(v0) << std::endl;
    return 0;
}