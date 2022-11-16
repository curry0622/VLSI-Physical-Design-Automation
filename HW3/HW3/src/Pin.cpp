#include "Pin.hpp"

Pin::Pin() {
    name = "default";
    x_cord = -1;
    y_cord = -1;
}

Pin::Pin(std::string n, int x, int y) {
    name = n;
    x_cord = x;
    y_cord = y;
}

void Pin::print() {
    std::cout << "---" << std::endl;
    std::cout << "Pin name: " << name << std::endl;
    std::cout << "Pin x_cord: " << x_cord << std::endl;
    std::cout << "Pin y_cord: " << y_cord << std::endl;
    std::cout << "---" << std::endl;
}