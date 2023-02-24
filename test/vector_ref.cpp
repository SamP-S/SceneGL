#include <iostream>
#include <vector>
#include <unistd.h>

void dummy(std::vector<int> arr) {
        std::cout << "dummy" << std::endl;
        std::cout << &arr << std::endl;
        std::cout << arr[0] << ":" << &arr[0] << std::endl;
        std::cout << arr[1] << ":" << &arr[1] << std::endl;
}

void idummy(int a) {
        std::cout << "d:" << a << ":" << &a << std::endl;
}

std::vector<float> x;

int main() {
        
        std::cout << "main" << std::endl;
        // int a = 4;
        // std::cout << "a:" << a << ":" << &a << std::endl;
        // idummy(a);
	std::vector<int> arr = std::vector<int>();
        arr.push_back(3);
        arr.push_back(4);
        arr.push_back(5);
	std::cout << arr.size() << std::endl;
	std::cout << &arr << std::endl;
        std::cout << arr[0] << ":" << &arr[0] << std::endl;
        std::cout << arr[1] << ":" << &arr[1] << std::endl;
	dummy(arr);
}
