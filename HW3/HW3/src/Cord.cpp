#include "Cord.hpp"

Cord::Cord() {
    x = -1;
    y = -1;
}

Cord::Cord(int x, int y) {
    this->x = x;
    this->y = y;
}

void Cord::print() {
    std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
}
