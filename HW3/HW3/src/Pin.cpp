#include "Pin.hpp"

Pin::Pin() {
    name = "default";
    cord = Cord();
}

Pin::Pin(std::string n, int x, int y) {
    name = n;
    cord = Cord(x, y);
}

void Pin::print() {
    std::cout << "---" << std::endl;
    std::cout << "Pin name: " << name << std::endl;
    std::cout << "Pin cord: ";
    cord.print();
    std::cout << "---" << std::endl;
}