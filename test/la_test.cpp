#include <iostream>
#include <stdio.h>

#include "la.hpp"
using namespace LA;


int main() {
    printf("ahh\n");
    vec3 v = vec3(1.0f);
    // printf("%f, %f, %f", v.x, v.y, v.z);
    std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}