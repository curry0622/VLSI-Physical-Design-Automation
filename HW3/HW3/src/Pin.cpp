#include "Pin.hpp"

Pin::Pin() {
    name = "default";
    coord = Coord();
}

Pin::Pin(std::string n, int x, int y) {
    name = n;
    coord = Coord(x, y);
}

void Pin::print() {
    std::cout << "---" << std::endl;
    std::cout << "Pin name: " << name << std::endl;
    std::cout << "Pin coord: ", coord.print();
    std::cout << "---" << std::endl;
}