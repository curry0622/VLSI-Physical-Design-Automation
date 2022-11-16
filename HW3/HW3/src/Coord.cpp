#include "Coord.hpp"

Coord::Coord() {
    x = -1;
    y = -1;
}

Coord::Coord(int x, int y) {
    this->x = x;
    this->y = y;
}

void Coord::print() {
    std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
}
